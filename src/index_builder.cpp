/*
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#include <ctime>
#include <fstream>
#include <iostream>

#include <dirent.h>

#include "crawler_info.hpp"
#include "index_builder.hpp"
#include "parser.hpp"

namespace wunner
{

  Index::Index(IndexInfo ii)
  {
      std::cout << "Indexing..." << std::endl;
      if (ii == IndexInfo::BUILD_INDEX) {
          build_index();
      } else {
          read_index();
      }
  }

  void Index::parse_docs()
  {
      std::cout << "Parsing crawled documents...\n";
      Parser p;

      DIR *dir = opendir(CRAWLED);
      struct dirent *walk;
      if (dir) {
          while ((walk = readdir(dir)) != NULL) {
              if (std::string(walk->d_name) == "." || std::string(walk->d_name) == "..") {
                  continue;
              }
              auto doc = p.get_parsed_document(std::string(CRAWLED) + "/" + walk->d_name);
              parsed_docs[walk->d_name] = doc;
              std::cout << "Parsed document " << walk->d_name << std::endl;
          }
          closedir(dir);
          std::cout << "Parsing complete!\n";

          // write the parsed documents list to a file
          std::ofstream fout(INDEXED_DOCS);
          if (fout) {
              for (auto & doc : parsed_docs) {
                  fout << doc.first << " " << doc.second.size() << " ";
                  for (auto & word : doc.second) {
                      fout << word << " ";
                  }
              }
              fout.close();
          } else {
              throw std::runtime_error("Couldn't open list of parsed documents!!");
          }
          std::cout << "Saved parsed documents list\n";
      } else {
          throw std::runtime_error("Directory containing crawled documents not present or corrupt");
      }
  }

  void Index::build_index()
  {
      std::cout << "Building a fresh index...\n";
      parse_docs();
      for (auto & doc : parsed_docs) {
          int pos = 0;
          for (auto & word : doc.second) {
              inverted_index[word].push_back(std::make_pair(doc.first, pos));
              pos++;
          }
      }

      std::ofstream fout(FILENAME);    // Serializing the index and storing it to a file for further reference
      if (fout) {
          for (auto & word_info : inverted_index) {
              fout << word_info.first << " " << word_info.second.size() << " ";
              for (auto & place : word_info.second) {
                  fout << place.first << " " << place.second << " ";
              }
          }
          fout.close();
      } else {
          throw std::runtime_error{"Couldn't save the index!!"};
      }
      std::cout << "Built index successfully!\n";
  }

  void Index::read_index()
  {
      std::ifstream fin;
      fin.open(FILENAME);
      std::string key, doc_num;
      int length;
      ll pos;
      while (fin >> key >> length) {
          while (length--) {
              fin >> doc_num >> pos;
              inverted_index[key].push_back(std::make_pair(doc_num, pos));
          }
      }
      fin.close();

      fin.open(INDEXED_DOCS);
      std::string word;
      while (fin >> key >> length) {
          while (length--) {
              fin >> word;
              parsed_docs[key].push_back(word);
          }
      }
      fin.close();
  }

  std::vector<std::string> & Index::fetch_parsed_document(std::string const & doc_hash)
  {
      return parsed_docs[doc_hash];
  }

  std::vector<std::pair<std::string, ll>> & Index::get_index(std::string const & word)
  {
      return inverted_index[word];
  }
}
