/*
 *
 * Author : Anishka Gupta
 * Created on : July 8, 2017
 *
 */

#define long long ll

#include <algorithm>
#include <cmath>

#include "query.hpp"

namespace wunner
{
  Query::Query(std::string const & query)
  {
      Parser p;
      processed_query = p.get_parsed_query(query);
  }

  const std::vector<std::string> const & Query::get_processed_query() const
  {
      return processed_query;
  }

  const std::set<std::string> & Query::get_union_docs() const
  {
      // returns a set of all those documents which contain the occurence of even one word from the parsed query
      for (auto & word : processed_query) {
          for (auto & res : i.get_index(word)) {
              union_docs.push_back(res.first);
          }
      }
      return union_docs;
  }
  
  QueryRanker::QueryRanker(std::string const & query, double k1 = 2.0, double b = 0.75)
  {
      Query q(query);
      std::set<std::string> docs = q.get_union_docs();
      for (auto & doc_name : docs) {
          auto & doc = q.i.fetch_parsed_document(doc_name);
          doc_info[doc_name].first = doc.size();
          for (word : q.get_processed_query()) {
              doc_info[doc_name].second.push_back(std::count(doc.begin(), doc.end(), word));
          }
      }

      avg_doc_len = 0;
      for (auto & d : doc_info) {
          avg_doc_len += d.second.first;
      }
      avg_doc_len /= double(doc_info.size());
      this->k1 = k1;
      this->b = b;

      for (int j = 0; j < q.get_processed_query.size(); j++) {
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

  std::vector<std::pair<int, std::string>> QueryRanker::fetch_ranked_list()
  {
      for (auto & doc : doc_info) {
          double doc_score = 0;
          for (auto j = 0; j < doc.second.second.size(); j++) {
              double tf = double(doc.second.second[j]);
              double f = double(all_freq[j]);
              ll doc_len = doc.second.first;
              doc_score += get_IDF(doc_info.size(), f) * get_MFrac(tf, doc_len);
          }
          ranked_docs.push_back(doc_score);
      }

      return ranked_docs;
  }

}
