#ifndef _DKNB_STRING_UTIL_SEARCH_HPP_
#define _DKNB_STRING_UTIL_SEARCH_HPP_

#include <string>
#include <utility>
#include <vector>

namespace dknb {
namespace string_util {

int find_next_head(const std::string& str, const std::string& ptn, int begin) {
  for (uint i = begin; i < str.size(); ++i) {
    if (str[i] == ptn[0]) {
      return i;
    }
  }
  return -1;
}

std::pair<bool, int> find_next_substr(const std::string& str,
                                      const std::string& ptn, int begin) {
  int pos = find_next_head(str, ptn, begin);
  if (pos == -1 || str.size() < begin + ptn.size()) {
    return {false, -1};
  }

  for (uint i = 0; i < ptn.size(); ++i) {
    if (str[pos + i] != ptn[i]) {
      return {false, pos + i + 1};
    }
  }
  return {true, pos + 1};
}

int find(const std::string& str, const std::string& ptn) {
  int pos = 0;
  while (pos != -1) {
    std::pair<bool, int> res = find_next_substr(str, ptn, pos);
    pos = res.second;
    if (res.first) {
      return pos - 1;
    }
  }
  return -1;
}

int count_substr(const std::string& str, const std::string& ptn) {
  int count = 0;

  std::pair<bool, int> m = find_next_substr(str, ptn, 0);
  bool found = m.first;
  int next_pos = m.second;

  if (found) {
    count++;
  }

  while (next_pos != -1) {
    m = find_next_substr(str, ptn, next_pos);
    found = m.first;
    next_pos = m.second;
    if (found) {
      count++;
    } else {
      if (next_pos == -1) {
        break;
      }
    }
  }

  return found ? count + 1 : count;
}

}  // namespace string_util
}  // namespace dknb

#endif  // _DKNB_STRING_UTIL_SEARCH_HPP_
