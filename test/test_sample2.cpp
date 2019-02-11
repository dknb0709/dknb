#include <sample2.hpp>
#include <cassert>
#include <iostream>

int main() {
  std::cout << "[test] start" << std::endl;

  person p1("falco", 42);
  person p2("malco", 41);
  person p3("dalco", 42);
  person p4("dalco", 43);

  assert(p1 > p2);
  assert(p1 == p3);
  assert(p1 < p4);
  
  std::cout << "[test] finish" << std::endl;
}

