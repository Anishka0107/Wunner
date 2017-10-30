/*
 *
 * Author : Anishka Gupta
 * Created on : July 8, 2017
 *
 */

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

#include "crawler.hpp"
#include "parser.hpp"
#include "query.hpp"
#include "utils.hpp"

namespace wunner
{

  Query::Query(std::string const & query, Index *idx)
  {
      Parser p;
      processed_query = p.get_parsed_query(query);
      i = idx;
  }

  const std::vector<std::string> & Query::get_processed_query() const
  {
      return processed_query;
  }

  const std::set<std::string> & Query::get_union_docs()
  {
      // returns a set of all those documents which contain the occurrence of even one word from the parsed query
      for (auto & word : processed_query) {
          for (auto & res : (*i).get_index(word)) {
              union_docs.insert(res.first);
          }
      }
      return union_docs;
  }
  
  QueryRanker::QueryRanker(Query & q) : QueryRanker(q, 2.0, 0.75) {}
  
  QueryRanker::QueryRanker(Query & q, double k1, double b)
  {
      auto docs = q.get_union_docs();
      auto processed_query = q.get_processed_query();
      for (auto & doc_name : docs) {
          auto doc = q.i->fetch_parsed_document(doc_name);
          doc_info[doc_name].first = doc.size();
          for (auto & word : processed_query) {
              doc_info[doc_name].second.push_back(std::count(doc.begin(), doc.end(), word));
          }
      }

      avg_doc_len = 0.0;
      for (auto & d : doc_info) {
          avg_doc_len += d.second.first;
      }
      avg_doc_len /= double(doc_info.size());
      this->k1 = k1;
      this->b = b;

      for (size_t j = 0; j < processed_query.size(); j++) {
          ll f = 0;
          for (auto & doc : doc_info) {
              if (doc.second.second[j]) {
                  f++;
              }
          }
          all_freq.push_back(f);
      }
  }

  double QueryRanker::get_IDF(double N, double n)
  {
      double r = log((N - n + 0.5) / (n + 0.5));
      if (r >= 0)
          return r;
      return 0;
  }

  double QueryRanker::get_MFrac(double tf, double doc_len)
  {
      return (tf * (k1 + 1)) / (tf + k1 * (1 - b + b * (doc_len / avg_doc_len)));
  }

  std::vector<std::pair<double, std::string>> QueryRanker::fetch_ranked_list()
  {
      for (auto & doc : doc_info) {
          double doc_score = 0;
          for (size_t j = 0; j < doc.second.second.size(); j++) {
              double tf = double(doc.second.second[j]);
              double f = double(all_freq[j]);
              ll doc_len = doc.second.first;
              doc_score += get_IDF(doc_info.size(), f) * get_MFrac(tf, doc_len);
          }
          ranked_docs.push_back(std::make_pair(doc_score, doc.first));
      }

      return ranked_docs;
  }

  CombinedPageRank::CombinedPageRank(std::vector<std::pair<double, std::string>> & ranked_list)
  {
      std::ifstream fin(PAGE_RANKS);
      std::unordered_map<std::string, double> page_ranks;
      bool use_page_rank = true;

      if (!fin) {
          std::cerr << "Page ranks not computed or file missing/corrupted! Using fallback query based ranks\n";
          use_page_rank = false;
      }

      std::string page, rank;       // reading in string and parsing later to double, to check wrong values (eg: inf) 
      while (fin >> page >> rank) {
          try {
              page_ranks[page] = std::stod(rank);    // ideally, no exceptions would occur
          } catch (std::exception & ex) {        // case when file is edited or corrupted
              std::cerr << "WARNING: Page rank file content modified! Using fallback query based ranks\n";
              use_page_rank = false;
          }
      }
      fin.close();

      if (use_page_rank) {
          for (auto & bm25_ranked : ranked_list) {
              double par1 = bm25_ranked.first;
              double par2 = page_ranks[bm25_ranked.second];    // use index directly, expected to be present in map
              bm25_ranked.first = (par1 * par2) / (par1 + par2);      // harmonic mean
          }
      }

      sort(ranked_list.begin(), ranked_list.end(), sort_comparator<double>);

      fin.open(CRAWLED_IDS);

      if (!fin) {
          throw std::runtime_error("Error! Cannot find crawled docs IDs!");
      }

      std::unordered_map<std::string, std::string> url_map;
      std::string url_id, url;
      while (fin >> url_id >> url) {
          url_map[url_id] = url;
      }
      fin.close();

      for (auto & ranked : ranked_list) {            // show all results; later on if size grows, can add functionality to omit few last urls
          auto iter = url_map.find(ranked.second);
          if (iter == url_map.end()) {
              throw std::runtime_error("Page ID to URL mapping not found! Error in crawled doc IDs");
          }
          final_ranked_list.push_back(iter->second);
      }
  }

  std::vector<std::string> & CombinedPageRank::get_final_ranked_list()
  {
      return final_ranked_list;
  }

}
