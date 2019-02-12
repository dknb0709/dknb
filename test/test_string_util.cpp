#include <cassert>
#include <iostream>
#include <string_util/string_util.hpp>

void test_split() {
  std::vector<std::string> actual = split("aaa,bbb,ccc", ",");
  std::vector<std::string> expected = { "aaa", "bbb", "ccc" };
  int i = 0;
  for (std::string s : actual) {
    assert(s == expected[i++]);
  }
}

void test_find() {
  
  std::string str = "ABCDEFG", ptn = "DEF";
  int pos = find(str, ptn);
  assert(pos == 3);

  str = "ABCDEFG"; ptn = "DEFH";
  pos = find(str, ptn);
  assert(pos == -1);

}

int main() {
  test_split();
  test_find();
}

