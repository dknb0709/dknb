#include <iostream>
#include <string>
#include <vector>
#include <regex>

std::vector<std::string> split(std::string&& src, std::string&& delim) {
  std::regex re(delim);
  std::sregex_token_iterator first(src.begin(), src.end(), re, -1), last;
  return std::vector<std::string>(first, last);
}

int main() {
  std::vector<std::string> ret = split("aaa,bbb,ccc", ",");
  for (std::string s : ret) {
    std::cout << s << std::endl;
  }
}

