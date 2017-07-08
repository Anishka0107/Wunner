/*
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#define long long ll

#ifndef MIN_DIFF
#define MIN_DIFF 1000000
#endif  // MIN_DIFF

#ifndef FILENAME
#define FILENAME "index_file"
#endif  // FILENAME

#ifndef CRAWLED
#define CRAWLED "crawled_dir"
#endif  // CRAWLED

#ifndef BUILD_INVERTED_INDEX
#define BUILD_INVERTED_INDEX

#include <pair>
#include <string>
#include <unordered_map>

namespace wunner
{
  class Index
  {
      private:
          // timestamping
          std::unordered_map<std::string, std::vector<std::pair<ll, ll>>> inverted_index;
          const std::vector<std::vector<std::string>> & parse_docs() const;
          void build_index() const;

      public:
          Index();
          ~Index();
          const std::vector<std::pair<ll, ll>> & get_index(std::vector<std::string> const &) const;
  };
}

#endif  // BUILD_INVERTED_INDEX
