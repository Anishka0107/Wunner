/**
 *
 * Author : Anishka Gupta
 * Created on : July 8, 2017
 *
 */

#include <functional>

#include <boost/regex.hpp>
#include <curl/curl.h>

#include "wunner/crawler.hpp"

namespace wunner
{

  const std::string & Crawler::get_id(std::string const & url_name)
  {
      size_t hash = std::hash<std::string>{}(url_name);
      return std::to_string(hash);
  }

  void Crawler::fetch_page_text(std::string const & url, std::string const & page) const
  {
      CURLcode code(CURLE_FAILED_INIT);
      CURL *curl = curl_easy_init();
      if (curl) {
          curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
          curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
          curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
          curl_easy_setopt(curl, CURLOPT_TIMEOUT, 56L)
          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, &page);

          if (curl_easy_perform(curl) == CURLE_OK) {
              curl_easy_cleanup(curl);
          }
      }
  }

  void Crawler::crawl()         // remember to delete all stuff already crawled when this method is called
  {
      std::ifstream fin(CRAWL_SEED_SRC);
      std::queue<std::string> urls;
      while (!fin.eof()) {
          std::string url;
          fin >> url;
          urls.push(url);
      }

      int curr_crawl_page = 0;
      std::ofstream fout(CRAWLED_IDS);

      while (!urls.empty() && curr_crawl_page < CRAWL_NUM_PAGE) {
          std::string url = urls.front();
          urls.pop();

          if (visited.find(url) == visited.end()) {
              std::string page;
              fetch_page_text(url, page);
              std::string url_id = get_id(url);
              fout << url_id << " " << url << endl;

              std::ofstream write_file(CRAWLED + "/" + url_id);
              write_file << page;
              write_file.close();

              const boost::regex link_expr("<a\s+(?:[^>]*?\s+)?href=([\"'])(.*?)\1");
              boost::match_results<std::string::const_iterator> res;
              auto start = page.begin();
              while (regex_search(start, page.end(), res, link_expr)) {
                  q.push(res[0]);
                  start = res[0].second;
              }

              visited.insert(url);
              curr_crawl_page++;
          }
      }
      fout.close();
  }

  size_t write_data(char* buf, size_t size, size_t nmemb, void* userp)
  {
      (std::string*(userp))->append(buf, size * nmemb);
      return size * nmemb;
  }

}

