/*
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#ifndef CRAWLER
#define CRAWLER "crawled_dir"
#endif  // CRAWLER

#ifndef STOPPER
#define STOPPER "stop_words"
#endif  // STOPPER

#ifndef PARSER
#define PARSER

#include <string>
#include <unordered_set>
#include <vector>

namespace wunner
{
  class Parser
  {
      private:
          std::vector<std::string> parsed_doc;
          std::unordered_set<std::string> stop_words;

          void tokenizer(std::string const &);
          void query_tokenizer(std::string const &);
          void normalizer();
          void stop_words_removal();
          void stemmer();

      public:
          Parser();
          std::vector<std::string> get_parsed_document(std::string const &);
          const std::vector<std::string> & get_parsed_query(std::string const &);
  };
}

#endif  // PARSER
