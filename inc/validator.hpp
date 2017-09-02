/**
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#ifndef ERRONEOUS
#define ERRONEOUS "res/erroneous"
#endif  // ERRONEOUS

#ifndef WORD_LIST
#define WORD_LIST "res/word_list"
#endif  // WORD_LIST

#ifndef MAX_SUGGESTIONS
#define MAX_SUGGESTIONS 7
#endif  // MAX_SUGGESTIONS

#ifndef WUNNER_VALIDATOR_HPP
#define WUNNER_VALIDATOR_HPP

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace wunner
{

  class Validator {
      private:
          size_t len;
          std::unordered_map<std::string, std::vector<std::string>> word_list;
          std::unordered_multimap<std::string, std::string> erroneous;

          void generate_n_grams(std::string const &);
          std::vector<std::pair<int, std::string>> get_n_grams(std::string const &);
          int compare_strings(std::string const &, std::string const &) const;

      public:
          Validator();
          Validator(int);
          std::list<std::string> suggest(std::string const &);
  };

}

#endif  // WUNNER_VALIDATOR_HPP
