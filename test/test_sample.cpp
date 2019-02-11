#include <sample.hpp>
#include <cassert>
#include <iostream>

int main() {
  std::cout << "[test] start" << std::endl;

  Person person("falco", 42);
  std::string name = person.get_name();

  assert(name == "falco");

  std::cout << "[test] finish" << std::endl;
}

