/*
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#define long long ll

#ifndef MIN_DIFF
#define MIN_DIFF 604800  // a week
#endif  // MIN_DIFF

#ifndef FILENAME
#define FILENAME "index_file"
#endif  // FILENAME

#ifndef CRAWLED
#define CRAWLED "../res/crawled_dir"
#endif  // CRAWLED

#ifndef INDEXED_DOCS
#define INDEXED_DOCS "indexed_documents"
#endif  // INDEXED_DOCS

#ifndef BUILD_INVERTED_INDEX
#define BUILD_INVERTED_INDEX

#include <pair>
#include <string>
#include <unordered_map>
#include <vector>

namespace wunner
{
  class Index
  {
      private:
          // timestamping
          std::unordered_map<std::string, std::vector<std::pair<ll, ll>>> inverted_index;
          const std::unordered_map<std::string, std::vector<std::string>> parsed_docs;
          void parse_docs() const;
          void build_index() const;

      public:
          Index();
          ~Index();
          std::vector<std::pair<ll, ll>> & get_index(std::string const &) const;
          std::vector<std::string> & fetch_parsed_document(std::string const &) const;
  };

}

#endif  // BUILD_INVERTED_INDEX
