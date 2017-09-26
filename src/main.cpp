/*
 *
 * Author : Anishka Gupta
 * Created on : July 28, 2017
 *
 */

#include <condition_variable>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <sys/stat.h>
#include <thread>

#include "autocomplete.hpp"
#include "crawler.hpp"
#include "query.hpp"
#include "validator.hpp"

std::mutex _mutex;
std::condition_variable _cv;
bool writing_index = false;

void search_for(wunner::Index *index, wunner::Autocomplete & autocomplete)
{
    while (true) {
        std::cout << "Enter your query : ";
        std::string query;
        std::cin >> query;

        while (query.back() == '\a') {                 // for autocomplete, use audible bell `Ctrl+G` and press `Enter`
            std::vector<std::string> query_list = autocomplete.search_for(query);  // starts complete search from root itself each time -> reasoning is that the user might want to change the prefix of the query in between (need to implement this feature). One way is to store a pointer to the first modified index, but to avoid complexity, not doing it, but it can be marked as a todo
            if(query_list.size() == 0) {
                std::cout << "No results found! Searching for the above query itself...";
                break;
            }
            for (size_t i = 0; i < query_list.size(); i++) {                     // shows a maximum of 10 suggestions numbered 0-9
                std::cout << i << ". " << query_list[i] << std::endl;
            }
            size_t choice;
            std::cin >> choice;
            if (choice < query_list.size()) {
                query += query_list[choice];
                std::cout << query;
                std::string remaining_query;
                std::cin >> remaining_query;
                query += remaining_query;
            } else {
                query.pop_back();
                std::cout << "Invalid choice! Please press `Ctrl + G` again to restart autocomplete, else continue typing your query";
            }
        }

        std::unique_lock<std::mutex> lock(_mutex);
        while (writing_index) {
            _cv.wait(lock);
        }
        autocomplete.submit_new_query(query);

        wunner::Query q(query, index);
        wunner::QueryRanker qr(q);
        auto rl = qr.fetch_ranked_list();
        wunner::CombinedPageRank cpr(rl);
        auto cpr_list = cpr.get_final_ranked_list();     // outputs sorted list based on rank; the strings are document URLs, not hashes
 
        if (cpr_list.empty()) {            // if even a single page is found, consider query valid, so no suggestions
            std::cout << "No results found! :(\n";
            wunner::Validator validator;
            auto processed_query = q.get_processed_query();
            int count = 0;
            for (auto it = processed_query.begin(); it != processed_query.end(); it++) {
                auto suggestions = validator.suggest(*it);
                for (auto & suggestion : suggestions) {
                    std::cout << "By " << *it << ", did you mean " << suggestion;
                    if (++count > MAX_SUGGESTIONS) {
                        it = processed_query.end() - 1;
                        break;
                    }
                }
            }
            return;
        }

        std::cout << "The following results were generated :\n";
        for (auto & page : cpr_list) {
            std::cout << page << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1 && (std::string(argv[1]) == "-f" || std::string(argv[1]) == "--fresh")) {
        std::system(("rm -rf " + std::string(CRAWLED)).c_str());
        std::cout << "A fresh start :)" << std::endl;
   }

    wunner::Crawler crawler;
    wunner::Index *index;
    wunner::Autocomplete autocomplete;

    std::time_t current_time = std::time(0);
    struct stat res;

    if (!stat(CRAWLED, & res) && (current_time - res.st_mtime < MIN_DIFF)) {
        index = new wunner::Index(wunner::IndexInfo::READ_INDEX);
    } else {
        crawler.crawl();
        index = new wunner::Index(wunner::IndexInfo::BUILD_INDEX);
    }
    search_for(index, autocomplete);

    auto refresh_index = [&](auto duration)
        {
            std::this_thread::sleep_for(duration);

            crawler.crawl();
            wunner::Index *index_temp = new wunner::Index(wunner::IndexInfo::BUILD_INDEX);

            std::unique_lock<std::mutex> lock(_mutex);
            writing_index = true;

            delete index;
            index = index_temp;
            std::cout << "Refreshed index!!\n";

            _cv.notify_all();
        };

    std::thread thread_for_search(& search_for, index, std::ref(autocomplete));
    std::thread thread_for_refreshing_index(refresh_index, std::chrono::seconds(MIN_DIFF));
    thread_for_search.join();
    thread_for_refreshing_index.join();

    return 0;
}
