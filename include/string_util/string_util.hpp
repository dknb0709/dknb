#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <utility>

#include "../type_util/type_util.hpp"

std::vector<std::string> split(std::string&& src, std::string&& delim) {
  std::regex re(delim);
  std::sregex_token_iterator first(src.begin(), src.end(), re, -1), last;
  return std::vector<std::string>(first, last);
}

int find_next_head(const std::string& str, const std::string& ptn, int begin) {
  for (uint i = begin; i < str.size(); ++i) {
    if (str[i] == ptn[0]) {
      return i;
    }
  }
  return -1;
}

std::pair<bool, int> find_next_substr(
    const std::string& str,
    const std::string& ptn,
    int begin) {

  int pos = find_next_head(str, ptn, begin);
  if (pos == -1 || str.size() < begin + ptn.size()) {
    return { false, -1 };
  }

  for (uint i = 0; i < ptn.size(); ++i) {
    if (str[pos + i] != ptn[i]) {
      return { false, pos + i + 1 };
    }
  }
  return { true, pos };
}

int find(const std::string& str, const std::string& ptn) {
  int pos = 0;
  while (pos != -1) {
    std::pair<bool, int> res = find_next_substr(str, ptn, pos);
    pos = res.second;
    if (res.first) {
      return pos;
    }
  }
  return -1;
}

int count_substr(const std::string& str, const std::string& ptn) {
  
  if (str.size() < ptn.size()) {
    return 0;
  }

  std::pair<bool, int> m = find_next_substr(str, ptn, 0);
  bool found = m.first;
  int next_find_pos = m.second;
  if (next_find_pos == -1) { // find_next_head hits nothing
    return 0;
  }
  
  int count = 0;
  while (next_find_pos != -1 && str.size() - next_find_pos < ptn.size()) {

    if (found) {
      ++count;
    }

    m = find_next_substr(str, ptn, next_find_pos);
    found = m.first;
    next_find_pos = m.second;

  }

  return count;
}

