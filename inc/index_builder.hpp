/*
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#ifndef ll
#define ll long long
#endif  // ll

#ifndef MIN_DIFF
#define MIN_DIFF 604800  // a week
#endif  // MIN_DIFF

#ifndef FILENAME
#define FILENAME "../res/index_file"
#endif  // FILENAME

#ifndef CRAWLED
#define CRAWLED "../res/crawled_dir"
#endif  // CRAWLED

#ifndef INDEXED_DOCS
#define INDEXED_DOCS "../res/indexed_documents"
#endif  // INDEXED_DOCS

#ifndef BUILD_INVERTED_INDEX
#define BUILD_INVERTED_INDEX

#include <string>
#include <unordered_map>
#include <vector>

namespace wunner
{

  enum class IndexInfo { BUILD_INDEX, READ_INDEX };

  class Index
  {
      private:
          // timestamping
          std::unordered_map<std::string, std::vector<std::pair<std::string, ll>>> inverted_index;
          std::unordered_map<std::string, std::vector<std::string>> parsed_docs;
          void parse_docs() const;
          void build_index();

      public:
          Index(IndexInfo);
          std::vector<std::string> & fetch_parsed_document(std::string const &);
          std::vector<std::pair<std::string, ll>> & get_index(std::string const &);
  };

}

#endif  // BUILD_INVERTED_INDEX
