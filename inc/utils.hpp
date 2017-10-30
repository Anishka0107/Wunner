/**
 *
 * Author : Anishka Gupta
 * Created on July 7, 2017
 *
 */

#ifndef WUNNER_UTILS_HPP
#define WUNNER_UTILS_HPP

namespace wunner {

  template <class T>
  bool sort_comparator(const std::pair<T, std::string> &p1, const std::pair<T, std::string> &p2)
  {
      return p1.first > p2.first;
  }

}

#endif  // WUNNER_UTILS_HPP
