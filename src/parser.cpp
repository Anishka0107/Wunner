/*
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <boost/tokenizer.hpp>

#include "parser.hpp"
#include "PorterStemmer/porter2_stemmer.h"

namespace wunner
{

  Parser::Parser()
  {
      std::ifstream fin(STOPPER);
      if (fin) {
          std::string stop_word;
          while (!fin.eof()) {
              getline(fin, stop_word);
              stop_words.insert(stop_word);
          }
      } else {
          // do nothing as of now
      }
  }
  
  void Parser::tokenizer(std::string const & document)
  {
      boost::char_separator<char> sep(",.@|-\"\"\'\' \n");
      std::ifstream fin(document);
      if (fin) {
          std::stringstream buf;
          buf << fin.rdbuf();
          std::string doc = buf.str();
          boost::tokenizer<boost::char_separator<char>> tokens(doc, sep);
          for (auto it = tokens.begin(); it != tokens.end(); ++it) {
              parsed_doc.push_back(*it);
          }
          fin.close();
      } else {
          // Do nothing, skip that file
          std::cerr << "Document " << document << " not found!" << std::endl;
      }
  }

  void Parser::query_tokenizer(std::string const & query)
  {
      boost::char_separator<char> sep(",.@|-\"\"\'\' \n");
      boost::tokenizer<boost::char_separator<char>> tokens(query, sep);
      for (auto it = tokens.begin(); it != tokens.end(); ++it) {
          parsed_doc.push_back(*it);
      }
  }

  void Parser::normalizer()
  {
      for (auto & word : parsed_doc) {
           std::transform(word.begin(), word.end(), word.begin(), ::tolower);
      }
  }

  void Parser::stop_words_removal()
  {
      parsed_doc.erase(std::remove_if(parsed_doc.begin(), parsed_doc.end(),
                                         [&](std::string const & w) {
                                             if (stop_words.find(w) == stop_words.end()) {
                                                 return false;
                                             } else {
                                                 return true;
                                             }
                                     }), parsed_doc.end());
  }

  void Parser::stemmer()
  {
      std::unordered_map<std::string, std::string> yet_stemmed;
      for (auto & word : parsed_doc) {
          if (yet_stemmed.find(word) == yet_stemmed.end()) {
              std::string orig_word = word;
              Porter2Stemmer::stem(word);
              yet_stemmed[orig_word] = word;
          } else {
              word = yet_stemmed[word];
          }
      }
  }

  std::vector<std::string> Parser::get_parsed_document(std::string const & document)
  {
      parsed_doc.clear();
      tokenizer(document);
      normalizer();
      stop_words_removal();
      stemmer();
      return parsed_doc;
  }

  const std::vector<std::string> & Parser::get_parsed_query(std::string const & query) 
  {
      parsed_doc.clear();
      query_tokenizer(query);
      normalizer();
      stop_words_removal();
      stemmer();
      return parsed_doc;
  }

}
