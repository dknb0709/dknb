#include <common/singleton.hpp>
#include <iostream>

template <class Derived>
class base : public dknb::singleton<Derived> {
  using singleton_t = dknb::singleton<Derived>;
public:
  static Derived& get_instance() {
    return singleton_t::get_instance();
  }
  virtual void perform() const = 0;
  // virtual void perform2() const = 0;
};

class sample : public base<sample> {
public:
  virtual void perform() const override {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
};

int main () {
  std::cout << "[test] start common/singleton test" << std::endl;
  sample::get_instance().perform();
  std::cout << "[test] finish common/singleton test" << std::endl;
}
