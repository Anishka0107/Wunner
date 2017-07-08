/**
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#include <algorithm>
#include <fstream>

#include "wunner/validator.hpp"

namespace wunner 
{

  Validator::Validator(int len = 2)
  {
      this->len = len;

      std::ifstream fin;
      fin.exceptions(std::ifstream failbit | ifstream::badbit);

      try {
          fin.open('word_list');
          while (!fin.eof()) {
              std::string word;
              fin >> word;
              generate_n_grams(word, len);
          }
          fin.close();
      } catch (const ifstream::failure & ex) {
          throw std::exception("Could not open or read the file \"word_file\"");
      }

      try {
          fin.open('erroneous');
          std::string incorrect;
          std::string correct;
          while (!fin.eof()) {
              fin >> incorrect;
              fin >> correct;
              erroneous[incorrect] = correct;
          }
          fin.close();
      } catch (...) {
          // do not throw error if it doesn't exist or is corrupt
      }
  }

  Validator::~Validator()
  {

  }

  void Validator::generate_n_grams(std::string const & word)
  {
      for (int i = 0; i < word.length() - len + 1; i++) {
          const std::string & gram = word.substr(i, len);
          word_list[gram].push_back(word);
      }
      if (word.length() < len) {
          word_list[word].push_back(word);
      }
  }

  std::unordered_set<std::pair<int, std::string>> Validator::get_n_grams(std::string const & word)
  {
      std::unordered_set<std::pair<int, std::string>> values;
      for (int i = 0; i < word.length() - len + 1; i++) {
          const std::string & gram = word.substr(i, len);
          auto val = word_list[gram];
          for (auto & j : val) {
              auto p = std::make_pair((int)0, j);
              values.insert(p);
          }
      }
      if (word.length() < len) {
          auto val = word_list[word];
          for (auto & j : val) {
              auto p = std::make_pair((int)0, j);
              values.insert(p);
          }
      }
      return values;
  }

  int Validator::compare_strings(std::string const & str1, std::string const & str2)
  {
      int dp[str1.length()+1][str2.length()+1];
      for (int i = 0; i <= str1.length(); i++) {
          dp[i][0] = i;
      }
      for (int i = 0; i <= str2.length(); i++) {
          dp[0][i] = i;
      }
      for (int i = 0; i <= str1.length(); i++) {
          for (int j = 0; j <= str2.length(); j++) {
              if (str1[i-1] == str2[j-1]) {
                  dp[i][j] = dp[i-1][j-1];
              } else {
                  dp[i][j] = 1 + min(dp[i][j-1], dp[i-1][j], dp[i-1][j-1]);
              }
          }
      }
      return dp[str1.length()][str2.length()];
  }

  std::list<std::string> Validator::suggest(std::string const & word)
  {
      std::list<std::string> suggestions;
      
      std::multimap<std::string, std::string>::iterator it;
      std::pair<it, it> res = erroneous.equal_range(word);

      for (it = res.first; it != res.second; it++) {
          suggestions.push_back(it->second);
      }

      auto values = get_n_grams(word);
      for (auto & value : values) {
          value.first = compare_strings(value.second, word);
      }
      sort(values.begin(), values.end(), sort_comparator);
      for (auto & value : values) {
          suggestions.push_back(value.second);
      }

      return suggestions;
  }

  bool sort_comparator(const std::pair<int, std::string> &p1, const std::pair<int, std::string> &p2)
  {
      return p1.first > p2.first;
  }

  int min(int a, int b, int c)
  {
      return min(a, min(b, c));
  }

}
