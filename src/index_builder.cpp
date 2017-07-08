/*
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#include <ctime>
#include <fstream>

#include <sys/stat.h>
#include <dirent.h>

#include "wunner/index_builder.hpp"
#include "wunner/parser.hpp"

namespace wunner
{

  Index::Index()
  {
      std::time_t current_time = std::time(0);
      struct stat res;
      int last_modified_time_of_index, last_modified_time_of_crawled_docs;

      if (!stat(FILENAME.c_str(), & res)) {
          last_modified_time_of_index = res.st_mtime;
      } else {
          build_index();
          continue;
      }

      if (!stat(CRAWLED.c_str(), & res)) {
          last_modified_time_of_crawled_docs = res.st_mtime;
      } else {
          // since couldn't open crawled docs directory, throw exception
          throw std::exception("Nothing to index, no directory containing crawled docs found!");
      }

      int time_diff = last_modified_time_of_crawled_docs - last_modified_time_of_index;
      if (time_diff > 0 && current_time - time_diff > MIN_DIFF) {
          build_index();
      } else {
          std::ifstream fin;
          fin.open(FILENAME, ios::in);
          std::string key,;
          int length;
          ll doc_num, pos;
          while (!fin.eof()) {
              fin >> key >> length;
              while (length--) {
                  fin >> doc_num >> pos;
                  inverted_index[key].push_back(std::make_pair(doc_num, pos));
              }
          }
          fin.close();
      }
  }

  const std::vector<std::pair<std::string, std::vector<std::string>>> & Index::parse_docs() const
  {
      std::vector<std::pair<std::string, std::vector<std::string>>> parsed_docs;
      Parser p;
      DIR *dir;
      struct dirent *walk;
      if ((dir = opendir(CRAWLED))) {
          while ((walk = readdir(dir)) {
              parsed_docs.push_back(p.get_parsed_document(walk->d_name));
          }
          closedir(dir);
      } else {
          throw std::exception("Directory containing crawled documents not present or corrupt");
      }
      return parsed_docs;
  }

  void Index::build_index()
  {
      auto parsed_docs = parse_docs();
      for (auto & doc : parse_docs) {
          int pos = 0;
          for (auto & word : doc.second) {
              inverted_index[word].push_back(make_pair(doc.first, pos));
              pos++;
          }
      }
      std::ofstream fout;
      fout.open(FILENAME, ios::out);
      // Serializing the index and storing it to a file for further reference
      for (auto & word_info : inverted_index) {
          fout << word_info.first << " " << word_info.second.size() << " ";
          for (auto & place : word_info.second) {
              fout << place.first << " " << place.second << " ";
          }
      }
      fout.close();
  }

  const std::vector<std::pair<ll, ll>> & get_index(std::vector<std::string> const & word) const
  {
      return inverted_index[word];
  }
}
