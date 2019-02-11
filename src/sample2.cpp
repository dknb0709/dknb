#include <sample2.hpp>

person::person(const std::string& name, int age)
  : name_(name)
  , age_(age)
{ }

int person::compare(const person& rhs) const {
  return age_ - rhs.age_;
}

