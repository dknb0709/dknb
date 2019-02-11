#include <string>

template <class T>
struct comparable {

  template <class U>
  friend bool operator==(const comparable& lhs, const U& rhs) {
    return static_cast<const T&>(lhs).compare(rhs) == 0;
  }

  template <class U>
  friend bool operator>(const comparable& lhs, const U& rhs) {
    return static_cast<const T&>(lhs).compare(rhs) > 0;
  }

  template <class U>
  friend bool operator<(const comparable& lhs, const U& rhs) {
    return static_cast<const T&>(lhs).compare(rhs) < 0;
  }

};

struct person : comparable<person> {
  std::string name_;
  int age_;
  person(const std::string& name, int age);
  int compare(const person& rhs) const;
};

