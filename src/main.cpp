
#define MAX_SUGGESTIONS 5

#include "wunner/crawler.hpp"
#include "wunner/query.hpp"

int main()
{
    Crawler crawler;
    crawler.crawl();
    Index index;
    while (true) {
        std::string query;
        std::cin >> query;

        Query q(query, index);
        std::vector<std::pair<int, std::string>> qr = QueryRanker(q);
        std::vector<std::string> cpr = CombinedPageRank(qr);
 
        if (cpr.empty()) {            // if even a single page is found, consider query valid, so no suggestions
            Validator validator;
            std::vector<std::string> processed_query = q.get_processed_query();
            int count = 0;
            for (auto it = processed_query.begin(); it != processed_query.end(); it++) {
                std::list<std::string> suggestions = validator.suggest(word);
                for (auto & suggestion : suggestions) {
                    cout << "By " << word << ", did you mean " << suggestion;
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
    return 0;
}
