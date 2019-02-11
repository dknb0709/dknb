#include "sample.hpp"

Person::Person(const std::string& name, unsigned age)
  : name_(name)
  , age_(age)
{ }

std::string Person::get_name() {
  return name_;
}

unsigned Person::get_age() {
  return age_;
}

