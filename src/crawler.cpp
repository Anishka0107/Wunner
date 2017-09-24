/**
 *
 * Author : Anishka Gupta
 * Created on : July 8, 2017
 *
 */

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <regex>

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
      std::system(("rm -rf " + std::string(CRAWLED) + " && mkdir " + std::string(CRAWLED)).c_str());    // delete already crawled stuff
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
      std::unordered_set<std::string> visited_ids;
      std::unordered_map<std::string, int> num_outbound;

      while (!urls.empty() && curr_crawl_page < CRAWL_NUM_PAGE) {
          std::string url = urls.front();
          urls.pop();

          if (visited.find(url) == visited.end()) {
              std::string url_id = get_id(url);
              std::string write_here = std::string(CRAWLED) + "/" + url_id;
              if (fetch_page_text(url, write_here)) {
                  std::cout << "Page NOT fetched " << url << "! ";
                  if (remove(write_here.c_str())) {
                      std::cout << "Deleted it..." << std::endl;
                  } else {
                      std::cout << "Not found / Couldn't delete it..." << std::endl;
                  }
                  continue;
              }

              std::cout << "Fetched " << url << std::endl;
              fout << url_id << " " << url << " ";

              std::ifstream fetch_file(write_here);
              std::string page;
              page.assign(std::istreambuf_iterator<char>(fetch_file), std::istreambuf_iterator<char>());
              fetch_file.close();

              std::ofstream put_file(write_here, std::ios::trunc);
              size_t ptr = 0;
              while (ptr < page.length()) {
                  while (ptr++ < page.length() && page[ptr] != '<') {
                      put_file << page[ptr];
                  }
                  while (++ptr < page.length() && page[ptr] != '>');
              }
              put_file.close();

              const std::regex link_expr("<a\\s+href=\"([\\-:\\w\\d\\.\\/]+)\">", std::regex_constants::icase);
              std::match_results<std::string::const_iterator> res;
              std::string::const_iterator start = page.begin(), end = page.end();
              int count_edges = 0;
              while (std::regex_search(start, end, res, link_expr)) {
                  urls.push(res[1]);
                  pr.add_edge(get_id(res[1]), url_id);        // as we care about incoming links, add edges in opposite order
                  start = res[0].second;
                  count_edges++;
              }

              visited.insert(url);
              visited_ids.insert(url_id);
              num_outbound[url_id] = count_edges;
              curr_crawl_page++;
          }
      }
      fout.close();

      pr.calculate_ranks(visited_ids, num_outbound);
      std::cout << "Crawling complete!!\n";
  }

  void PageRank::add_edge(std::string const & src, std::string const & dest)
  {
      adj_lst[src].insert(dest);
  }

  void PageRank::calculate_ranks(std::unordered_set<std::string> const & visited, std::unordered_map<std::string, int> & num_outbound)
  {
      const double damping_factor = 0.85;
      double initial_page_rank = 1.0 / visited.size();
      std::unordered_map<std::string, std::pair<double, double>> rank_list;     // pair.first --> original rank, pair.second --> temporary rank
      for (auto & node : visited) {
          rank_list.emplace(std::make_pair(node, std::make_pair(initial_page_rank, initial_page_rank)));
      }

      for (int i = 0; i < MAX_ITER_PAGE_RANK; i++) {
          for (auto & page : adj_lst) {
              if (visited.find(page.first) != visited.end()) {
                  double pr = 1 - damping_factor;
                  for (auto & link : page.second) {
                      pr += damping_factor * (rank_list[link].first / num_outbound[link]);
                  }
                  rank_list[page.first].second = pr;
              }
          }

          for (auto & page_rank : rank_list) {
              page_rank.second.first = page_rank.second.second;
          }
      }

      std::ofstream fout(PAGE_RANKS);
      if (fout) {
          for (auto & page_rank : rank_list) {
              fout << page_rank.first << " " << page_rank.second.first << " ";
          }
          fout.close();
      } else {
          std::cerr << "WARNING: Cannot save page ranks! Will have to resort to query based ranks!\n";
      }
  }

}
