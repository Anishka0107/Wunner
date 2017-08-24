/*
 *
 * Author : Anishka Gupta
 * Created on : July 8, 2017
 *
 */

#ifndef QUERY_PROCESSOR
#define QUERY_PROCESSOR

#include <map>
#include <set>
#include <string>
#include <vector>

#include "index_builder.hpp"

namespace wunner
{
  
  enum QueryType { SIMPLE, COMPLETE };
  
  class Query
  {
      private:
          std::vector<std::string> processed_query;
          std::set<std::string> union_docs;
      
      public:
          Index i;

          Query(std::string const &, Index &);
          ~Query();
          const std::set<std::string> & get_union_docs() const;
  };

  class QueryRanker
  {
      private:
          std::vector<std::pair<int, std::string>> ranked_docs;    // the int indicates the weight of the doc
          std::map<std::string, std::pair<int, std::vector<int>>> doc_info;  // needed for BM25 calculation
          std::vector<ll> all_freq;        // frequency of query words in bag of words
          double avg_doc_len;
          double k1;
          double b;

          double get_IDF(double, double);
          double get_MFrac(double, double);
          // remove TD-IDF, add Okapi-BM25 as it performs better

      public:
          QueryRanker(Query const &, double, double);
          std::vector<std::pair<double, std::string>> fetch_ranked_list();
  };

  class CombinedPageRank
  {
      private:
          std::vector<std::string> final_ranked_list;

      public:
          CombinedPageRank(std::vector<std::pair<double, std::string>> &);
          std::vector<std::string> & get_final_ranked_list();
  }
}

#endif  // QUERY_PROCESSOR
