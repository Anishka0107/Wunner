/**
 *
 * Author : Anishka Gupta
 * Created on : July 8, 2017
 *
 */

#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>

#include <boost/regex.hpp>

#include "crawler.hpp"

namespace wunner
{

  std::string Crawler::get_hash(std::string const & url_name)
  {
      size_t hash = std::hash<std::string>{}(url_name);
      return std::to_string(hash);
  }

  std::string Crawler::get_id(std::string const & url_name)
  {
      if (hashed.find(url_name) == hashed.end()) {
          hashed[url_name] = get_hash(url_name);
      }
      return hashed[url_name];
  }

  int Crawler::fetch_page_text(std::string const & url, std::string const & write_here) const
  {
      return std::system(("wget -O " + write_here + " " + url).c_str());            // a heavy system call :(
      // not using libcurl anymore, statically linking it is a pain, also it's a bit more complicated to write
  }

  void Crawler::crawl()
  {
      std::system(("rm -rf " + std::string(CRAWLED) + " && mkdir " + std::string(CRAWLED)).c_str());    // delete already crawled stuff  // don't care about rm warning if file does not exist
      std::cout << "Crawling..." << std::endl;
      std::ifstream fin(CRAWL_SEED_SRC);
      std::queue<std::string> urls;

      if (!fin) {
          throw std::runtime_error{"Cannot open CRAWL_SEED_SRC file! Needs seed to start crawling"};
      }

      while (fin) {
          std::string url;
          fin >> url;
          urls.push(url);
      }

      int curr_crawl_page = 0;
      std::ofstream fout(CRAWLED_IDS);

      PageRank pr;

      while (!urls.empty() && curr_crawl_page < CRAWL_NUM_PAGE) {
          std::string url = urls.front();
          urls.pop();

          if (visited.find(url) == visited.end()) {
              std::string url_id = get_id(url);
              std::string write_here = std::string(CRAWLED) + "/" + url_id;
              if (fetch_page_text(url, write_here)) {
                  std::cout << "Page NOT fetched " << url << std::endl;
                  continue;
              } else {
                  std::cout << "Fetched " << url << std::endl;
              }
              fout << url_id << " " << url << " ";

              std::ifstream fetch_file(write_here);
              std::string page;
              page.assign(std::istreambuf_iterator<char>(fetch_file), std::istreambuf_iterator<char>());
              fetch_file.close();

              const boost::regex link_expr("<a\\s+href=\"([\\-:\\w\\d\\.\\/]+)\">");
              boost::match_results<std::string::const_iterator> res;
              std::string::const_iterator start = page.begin(), end = page.end();
              while (regex_search(start, end, res, link_expr)) {
                  urls.push(res[0]);
                  pr.add_edge(get_id(res[0]), url_id);        // as we care about incoming links, add edges in opposite order
                  start = res[0].second;
              }

              visited.insert(url);
              curr_crawl_page++;
          }
      }
      fout.close();

      pr.calculate_ranks();
  }

  void PageRank::add_edge(std::string const & src, std::string const & dest)
  {
      adj_lst[src].insert(dest);    
  }

  void PageRank::calculate_ranks()
  {
      const double damping_factor = 0.85;
      double initial_page_rank = 1 / adj_lst.size();
      std::unordered_map<std::string, std::pair<double, double>> rank_list;     // pair.first --> original rank, pair.second --> temporary rank
      std::vector<double> temp_score(adj_lst.size(), initial_page_rank);
      for (auto & node : adj_lst) {
          rank_list.emplace(std::make_pair(node.first, std::make_pair(initial_page_rank, 0)));
      }

      for (int i = 0; i < MAX_ITER_PAGE_RANK; i++) {
          for (auto page : adj_lst) {
              double pr = 1 - damping_factor;
              for (auto & link : page.second) {
                  pr += damping_factor * (rank_list[link].first / adj_lst[link].size());
              }
              rank_list[page.first].second = pr;
          }

          for (auto & page_rank : rank_list) {
              page_rank.second.first = page_rank.second.second;
          }
      }

      for (auto & page_rank : rank_list) {
          std::ofstream fout(PAGE_RANKS);
          fout << page_rank.first << " " << page_rank.second.first << " ";
      }
  }

}

