/**
 *
 * Author : Anishka Gupta
 * Created on : July 8, 2017
 *
 */

#ifndef CRAWLED
#define CRAWLED "../res/crawled_dir"
#endif  // CRAWLED

#ifndef CRAWLED_IDS
#define CRAWLED_IDS "../res/crawled_ids"
#endif  // CRAWLED_IDS

#ifndef CRAWL_NUM_PAGE
#define CRAWL_NUM_PAGE 256
#endif  // CRAWL_NUM_PAGE

#ifndef CRAWL_SEED_SRC
#define CRAWL_SEED_SRC "../res/crawl_seed"
#endif  // CRAWL_SEED_SRC

#ifndef CRAWLER
#define CRAWLER

#include <string>
#include <unordered_set>

namespace wunner
{

  class Crawler
  {
      private:
          std::unordered_set<std::string> visited;

      public:
          const std::string & get_id(std::string const &);
          void fetch_page_text(std::string const &, std::string const &) const;
          void crawl();
  };
}

#endif  // CRAWLER
