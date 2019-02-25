#include <cassert>
#include <iostream>
#include <string_util/string_util.hpp>

using namespace dknb::string_util;

void test_split() {
  std::vector<std::string> actual = split("aaa,bbb,ccc", ",");
  std::vector<std::string> expected = {"aaa", "bbb", "ccc"};
  int i = 0;
  for (std::string s : actual) {
    assert(s == expected[i++]);
  }
}

void test_find_next_substr() {
  static int suite = 1;

  std::string str;
  std::string ptn;
  int begin;

  std::pair<bool, int> res;

  std::cout << "--- case" << suite++ << " ---" << std::endl;
  str = "ABCDEF";
  ptn = "ABC";
  begin = 0;
  res = find_next_substr(str, ptn, begin);
  assert(res.first == true);
  assert(res.second == 1);
  std::cout << "OK" << std::endl;

  std::cout << "--- case" << suite++ << " ---" << std::endl;
  str = "ABCDEF";
  ptn = "ABC";
  begin = 1;
  res = find_next_substr(str, ptn, begin);
  assert(res.first == false);
  assert(res.second == -1);
  std::cout << "OK" << std::endl;

  std::cout << "--- case" << suite++ << " ---" << std::endl;
  str = "ABCDEF";
  ptn = "DEF";
  begin = 0;
  res = find_next_substr(str, ptn, begin);
  assert(res.first == true);
  assert(res.second == 4);
  std::cout << "OK" << std::endl;

  std::cout << "--- case" << suite++ << " ---" << std::endl;
  str = "ABCDEF";
  ptn = "ABCDEFG";
  begin = 0;
  res = find_next_substr(str, ptn, begin);
  assert(res.first == false);
  assert(res.second == -1);
  std::cout << "OK" << std::endl;

  std::cout << "--- case" << suite++ << " ---" << std::endl;
  str = "AAA";
  ptn = "AA";
  begin = 0;
  res = find_next_substr(str, ptn, begin);
  assert(res.first == true);
  assert(res.second == 1);
  std::cout << "OK" << std::endl;

  std::cout << "--- case" << suite++ << " ---" << std::endl;
  str = "AAA";
  ptn = "AA";
  begin = 1;
  res = find_next_substr(str, ptn, begin);
  assert(res.first == true);
  assert(res.second == 2);
  std::cout << "OK" << std::endl;

  std::cout << "--- case" << suite++ << " ---" << std::endl;
  str = "AAA";
  ptn = "AA";
  begin = 2;
  res = find_next_substr(str, ptn, begin);
  assert(res.first == false);
  assert(res.second == -1);
  std::cout << "OK" << std::endl;
}

void test_find() {
  std::string str = "ABCDEFG", ptn = "DEF";
  int pos = find(str, ptn);
  assert(pos == 3);

  str = "ABCDEFG";
  ptn = "DEFH";
  pos = find(str, ptn);
  assert(pos == -1);
}

void test_count_substr() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  static int suite = 0;
  std::string str, ptn;
  int ret;

  std::cout << "--- case" << suite++ << " ---" << std::endl;
  str = "AAA";
  ptn = "AA";
  ret = dknb::string_util::count_substr(str, ptn);
  assert(ret == 2);
  std::cout << "OK" << std::endl;

  std::cout << "--- case" << suite++ << " ---" << std::endl;
  str = "ABABAB";
  ptn = "AB";
  ret = dknb::string_util::count_substr(str, ptn);
  assert(ret == 3);
  std::cout << "OK" << std::endl;

  std::cout << "--- case" << suite++ << " ---" << std::endl;
  str = "ABCDEFABCD";
  ptn = "ABCD";
  ret = dknb::string_util::count_substr(str, ptn);
  assert(ret == 2);
  std::cout << "OK" << std::endl;

  std::cout << "--- case" << suite++ << " ---" << std::endl;
  str = "ABABAB";
  ptn = "BA";
  ret = dknb::string_util::count_substr(str, ptn);
  assert(ret == 2);
  std::cout << "OK" << std::endl;
}

int main() {
  test_split();
  test_find();
  test_find_next_substr();
  test_count_substr();
}
