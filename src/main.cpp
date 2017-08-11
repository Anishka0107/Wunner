/*
 *
 * Author : Anishka Gupta
 * Created on : July 28, 2017
 *
 */

#include <sys/stat.h>
#include <thread>

#include "../inc/crawler.hpp"
#include "../inc/query.hpp"

void search_for(Index & i, std::mutex & mutex, std::condition_variable & cv)
{
    while (true) {
        std::unique_lock<std::mutex> lock(mutex);
        std::string query;
        std::cin >> query;

        cv.wait(lock);
        Query q(query, index);
        std::vector<std::pair<int, std::string>> qr = QueryRanker(q);
        std::vector<std::string> cpr = CombinedPageRank(qr);       // should output sorted list based on rank, and the strings should be document URLs, not hashes...
 
        if (cpr.empty()) {            // if even a single page is found, consider query valid, so no suggestions
            Validator validator;
            std::vector<std::string> processed_query = q.get_processed_query();
            int count = 0;
            for (auto it = processed_query.begin(); it != processed_query.end(); it++) {
                std::list<std::string> suggestions = validator.suggest(word);
                for (auto & suggestion : suggestions) {
                    std::cout << "By " << word << ", did you mean " << suggestion;
                    if (++count > MAX_SUGGESTIONS) {
                        it = processed_query.end() - 1;
                        break;
                    }
                }
            }
        }

        for (auto & page : cpr) {
            std::cout << crawler.get_page_url(page) << std::endl;
        }
    }
}

int main()
{
    Crawler crawler;
    std::unique_ptr<Index> index;

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
            Index index_temp(IndexInfo::BUILD_INDEX);

            std::unique_lock<std::mutex> lock(mutex);

            ii.~Index();
            ii = index_temp;          // shallow copy

            cv.notify_all();
            std::this_thread::sleep_for(duration);
        };

    std::thread thread_for_search(search_for(*index, mutex, cv));
    std::thread thread_for_refreshing_index(refresh_index(std::chrono::seconds(MIN_DIFF)));

    return 0;
}
