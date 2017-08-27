/*
 *
 * Author : Anishka Gupta
 * Created on : July 28, 2017
 *
 */

#include <iostream>
#include <sys/stat.h>
#include <thread>

#include "autocomplete.hpp"
#include "query.hpp"

void search_for(wunner::Index & i, wunner::Autocomplete & autocomplete, std::mutex & mutex, std::condition_variable & cv)
{
    while (true) {
        std::unique_lock<std::mutex> lock(mutex);
        std::string query;
        std::cin >> query;

        while (query.back() == '\a') {                 // for autocomplete, use audible bell `Ctrl+G` and press `Enter`
            std::vector<std::string> query_list = autocomplete.search_for(query);  // starts complete search from root itself each time -> reasoning is that the user might want to change the prefix of the query in between (need to implement this feature). One way is to store a pointer to the first modified index, but to avoid complexity, not doing it, but it can be marked as a todo
            if(query_list.size() == 0) {
                std::cout << "No results found! Searching for the above query itself...";
                break;
            }
            for (int i = 0; i < query_list.size(); i++) {                     // shows a maximum of 10 suggestions numbered 0-9
                std::cout << i << ". " << query_list[i] << std::endl;
            }
            int choice;
            std::cin >> choice;
            if (choice >= 0 && choice < query_list.size()) {
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

        cv.wait(lock);
        autocomplete.submit_new_query(query);
        wunner::Query q(query, i);
        wunner::QueryRanker qr(q);
        auto rl = qr.fetch_ranked_list();
        wunner::CombinedPageRank cpr(rl);
        auto cpr_list = cpr.get_final_ranked_list();     // outputs sorted list based on rank; the strings are document URLs, not hashes
 
        if (cpr_list.empty()) {            // if even a single page is found, consider query valid, so no suggestions
            wunner::Validator validator;
            auto processed_query = q.get_processed_query();
            int count = 0;
            for (auto it = processed_query.begin(); it != processed_query.end(); it++) {
                auto suggestions = validator.suggest(word);
                for (auto & suggestion : suggestions) {
                    std::cout << "By " << word << ", did you mean " << suggestion;
                    if (++count > MAX_SUGGESTIONS) {
                        it = processed_query.end() - 1;
                        break;
                    }
                }
            }
            return;
        }

        for (auto & page : cpr_list) {
            std::cout << page << std::endl;
        }
    }
}

int main()
{
    wunner::Crawler crawler;
    std::unique_ptr<Index> index;
    wunner::Autocomplete autocomplete;

    std::time_t current_time = std::time(0);
    struct stat res;

    if (!stat(CRAWLED.c_str(), & res) && (current_time - res.st_mtime < MIN_DIFF)) {
        index.reset(new Index(IndexInfo::READ_INDEX));
    } else {
        crawler.crawl();
        index.reset(new Index(IndexInfo::BUILD_INDEX));
    }

    std::mutex mutex;
    std::condition_variable cv;

    auto refresh_index = [&](auto duration)
        {
            crawler.crawl();
            wunner::Index index_temp(IndexInfo::BUILD_INDEX);

            std::unique_lock<std::mutex> lock(mutex);

            ii.~Index();
            ii = index_temp;          // shallow copy

            cv.notify_all();
            std::this_thread::sleep_for(duration);
        };

    std::thread thread_for_search(search_for(*index, autocomplete, mutex, cv));
    std::thread thread_for_refreshing_index(refresh_index(std::chrono::seconds(MIN_DIFF)));

    return 0;
}
