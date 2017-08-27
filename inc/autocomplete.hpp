/*
 *
 * Author : Anishka Gupta
 * Created on : August 26, 2017
 *
 */

#ifndef ALPHABET_SIZE
#define ALPHABET_SIZE 95
#endif  // ALPHABET_SIZE -> represents 32 - 126 ASCII characters

#ifndef ASCII_START_CODE
#define ASCII_START_CODE 32
#endif  // ASCII_START_CODE

#ifndef MAX_QUERIES
#define MAX_QUERIES 101
#endif  // MAX_QUERIES

#ifndef MAX_RESULTS
#define MAX_RESULTS 10
#endif  // MAX_RESULTS

#ifndef AUTOCOMPLETE
#define AUTOCOMPLETE

#include <queue>
#include <string>
#include <vector>

namespace wunner
{
  
  class Node
  {
      private:
          Node* children[ALPHABET_SIZE];
          bool is_leaf;

      public:
          Node();

      friend class Trie;
  };

  class Trie
  {
      private:
          Node* root;

      public:
          Trie();
          void insert(std::string const &);
          bool remove(std::string const &, Node*, int);
          std::vector<std::string> search(std::string const &);      // should return a maximum of MAX_RESULTS results
          void get_all_children(Node*, std::string &, std::vector<std::string> &);
  };
    
  class Autocomplete
  {
      // no files here to store old queries, as it is trivial
      private:
          Trie trie;
          std::queue<std::string> submitted_queries;

      public:
          void submit_new_query(std::string const &);
          std::vector<std::string> search_for(std::string const &);
  };

}

#endif  // AUTOCOMPLETE
