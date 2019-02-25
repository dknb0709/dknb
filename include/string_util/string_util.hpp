#ifndef _DKNB_STRING_UTIL_HPP_
#define _DKNB_STRING_UTIL_HPP_

#include <iostream>
#include <regex>
#include <string>
#include <string_util/predicate.hpp>
#include <string_util/search.hpp>
#include <utility>
#include <vector>

namespace dknb {
namespace string_util {

std::vector<std::string> split(std::string&& src, std::string&& delim) {
  std::regex re(delim);
  std::sregex_token_iterator first(src.begin(), src.end(), re, -1), last;
  return std::vector<std::string>(first, last);
}

}  // namespace string_util
}  // namespace dknb

#endif
