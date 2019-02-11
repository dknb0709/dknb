#include <string>

class Person {
  std::string name_;
  unsigned age_;
public:
  Person(const std::string& name, unsigned age);
  std::string get_name();
  unsigned get_age();
};

