/**
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#include <algorithm>
#include <fstream>

#include "utils.hpp"
#include "validator.hpp"

namespace wunner 
{

  Validator::Validator() : Validator(2) {}

  Validator::Validator(int len)
  {
      this->len = len;

      std::ifstream fin;
      fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      try {
          fin.open(WORD_LIST);
          std::string word;
          while (fin >> word) {
              generate_n_grams(word);
          }
          fin.close();
      } catch (std::ifstream::failure & ex) {
          throw std::runtime_error("Could not open or read the file \"word_file\"");
      }

      try {
          fin.open(ERRONEOUS);
          std::string incorrect, correct;
          while (fin >> incorrect >> correct) {
              erroneous.insert(std::make_pair(incorrect, correct));
          }
          fin.close();
      } catch (...) {
          // do not throw error if it doesn't exist or is corrupt
      }
  }

  int minimum(int a, int b, int c)
  {
      return std::min(a, std::min(b, c));
  }

  void Validator::generate_n_grams(std::string const & word)
  {
      for (size_t i = 0; i < word.length() - len + 1; i++) {
          const std::string & gram = word.substr(i, len);
          word_list[gram].push_back(word);
      }
      if (word.length() < len) {
          word_list[word].push_back(word);
      }
  }

  std::vector<std::pair<int, std::string>> Validator::get_n_grams(std::string const & word)
  {
      std::vector<std::pair<int, std::string>> values;
      for (size_t i = 0; i < word.length() - len + 1; i++) {
          const std::string & gram = word.substr(i, len);
          auto val = word_list[gram];
          for (auto & j : val) {
              values.push_back(std::make_pair(0, j));
          }
      }
      if (word.length() < len) {
          auto val = word_list[word];
          for (auto & j : val) {
              values.push_back(std::make_pair(0, j));
          }
      }
      return values;
  }

  int Validator::compare_strings(std::string const & str1, std::string const & str2) const
  {
      int dp[str1.length()+1][str2.length()+1];
      for (size_t i = 0; i <= str1.length(); i++) {
          dp[i][0] = i;
      }
      for (size_t i = 0; i <= str2.length(); i++) {
          dp[0][i] = i;
      }
      for (size_t i = 0; i <= str1.length(); i++) {
          for (size_t j = 0; j <= str2.length(); j++) {
              if (str1[i-1] == str2[j-1]) {
                  dp[i][j] = dp[i-1][j-1];
              } else {
                  dp[i][j] = 1 + minimum(dp[i][j-1], dp[i-1][j], dp[i-1][j-1]);
              }
          }
      }
      return dp[str1.length()][str2.length()];
  }

  std::list<std::string> Validator::suggest(std::string const & word)
  {
      std::list<std::string> suggestions;

      auto res = erroneous.equal_range(word);
      for (auto it = res.first; it != res.second; it++) {
          suggestions.push_back(it->second);
      }

      auto values = get_n_grams(word);
      for (auto & value : values) {
          value.first = compare_strings(value.second, word);
      }
      sort(values.begin(), values.end(), sort_comparator<int>);
      for (auto & value : values) {
          suggestions.push_back(value.second);
      }

      return suggestions;
  }

}
