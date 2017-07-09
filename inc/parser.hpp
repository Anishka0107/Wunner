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

#include <pair>
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

          void tokenizer(std::string const &) const;
          void query_tokenizer(std::string const &) const;
          void normalizer() const;
          void stop_words_removal() const;
          void stemmer() const;

      public:
          Parser();
          ~Parser();
          const std::pair<std::string, std::vector<std::string>> & get_parsed_document(std::string const &) const;
          const std::vector<std::string> & get_parsed_query(std::string const &) const;
  };
}

#endif  // PARSER
