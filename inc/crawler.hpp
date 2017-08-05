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
#endif  // TEMP_CRAWLED

#ifndef CRAWL_NUM_PAGE
#define CRAWL_NUM_PAGE 256
#endif  // CRAWL_NUM_PAGE

#ifndef MAX_CRAWL_DEPTH
#define MAX_CRAWL_DEPTH 7
#endif  // MAX_CRAWL_DEPTH

#ifndef CRAWL_SEED_SRC
#define CRAWL_SEED_SRC "../res/crawl_seed"
#endif  // CRAWL_SEED_SRC

#ifndef CRAWLER
#define CRAWLER

#include <set>
#include <string>

#include <curl/curl.h>

namespace wunner
{

  class Crawler
  {
      private:
          std::set<std::string> seed_urls;
          std::unordered_set<std::string> visited;

      public:
          void crawl();
  };
}

#endif  // CRAWLER
