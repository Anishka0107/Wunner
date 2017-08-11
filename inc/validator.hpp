/**
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#ifndef MAX_SUGGESTIONS
#ifndef MAX_SUGGESTIONS 7
#endif  // MAX_SUGGESTIONS

#ifndef WUNNER_VALIDATOR_HPP
#define WUNNER_VALIDATOR_HPP

#include <list>
#include <pair>
#include <string>
#include <unordered_map>
#include <unordered_multimap>
#include <unordered_set>
#include <vector>

namespace wunner
{

  class Validator {
      private:
          int len;
          std::unordered_map<std::string, std::vector<std::string>> word_list;
          std::unordered_multimap<std::string, std::string> erroneous;

          void generate_n_grams(std::string const &) const;
          std::unordered_set<std::pair<int, std::string>> get_n_grams(std::string const &) const;
          int compare_strings(std::string const &, std::string const &) const;

      public:
          Validator(int);
          ~Validator();
          std::list<std::string> suggest(std::string const &) const;
  };

}

#endif  // WUNNER_VALIDATOR_HPP
