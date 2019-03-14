#include <filesystem/directory.hpp>
#include <iostream>

int main() {
  dknb::Directory dir("aaa");
  std::cout << dir.getName() << std::endl;
}
