#include <cassert>
#include <string_util/string_util.hpp>

void test_split() {
  std::vector<std::string> ret = split("aaa,bbb,ccc", ",");
  for (std::string s : ret) {
    std::cout << s << std::endl;
  }
}

void test_find_next_submatch() {
  
  std::string str = "ABCDEFG", ptn = "DEF";
  int begin = 0;

  std::pair<bool, int> res = find_next_submatch(str, ptn, begin);
  assert(res.first == true);
  assert(res.second == 4);

  str = "ABCDEFG"; ptn = "DEF";
  begin = 4;

  res = find_next_submatch(str, ptn, begin);
  assert(res.first == false);
  assert(res.second == -1);

}

int main() {
  test_split();
  test_find_next_submatch();
}

