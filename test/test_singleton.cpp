#include <common/Singleton.hpp>
#include <iostream>

template <class Derived>
class base : public dknb::Singleton<Derived> {
  using singleton_t = dknb::Singleton<Derived>;

 public:
  static Derived& getInstance() { return singleton_t::getInstance(); }
  virtual void perform() const = 0;
  // virtual void perform2() const = 0;
};

class sample : public base<sample> {
 public:
  virtual void perform() const override {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
};

int main() {
  std::cout << "[test] start common/singleton test" << std::endl;
  sample::getInstance().perform();
  std::cout << "[test] finish common/singleton test" << std::endl;
}
