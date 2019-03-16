#include <property/property.hpp>
#include <iostream>

int main() {
  std::string ret =
      dknb::property::get("config::test.dknb.prop.normal").to_str();

  std::cout << "test: dknb::property::getInstance()" << std::endl;
  std::cout << dknb::property::getInstance() << std::endl;

  std::cout
      << "test: dknb::property::get(\"config::test.dknb.prop.normal\").to_str()"
      << std::endl;
  std::cout << ret << std::endl;
}
