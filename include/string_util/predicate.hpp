#ifndef _DKNB_STRING_UTIL_PREDICATE_HPP_
#define _DKNB_STRING_UTIL_PREDICATE_HPP_

#include <functional>

namespace dknb {
namespace string_util {

std::function<bool(char)> is_space = [](const char c) {
  return c == ' ' || c == '\n';
};
std::function<bool(char)> is_digit = [](const char c) {
  return c >= '0' && c <= '9';
};

}  // namespace string_util
}  // namespace dknb

#endif  // _DKNB_STRING_UTIL_PREDICATE_HPP_
