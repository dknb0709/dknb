#ifndef __DKNB_COMMON_SINGLETON_HPP__
#define __DKNB_COMMON_SINGLETON_HPP__

namespace dknb {

template <class T>
struct Singleton {
  static T& getInstance() {
    static T instance;
    return instance;
  }
};

}  // namespace dknb

#endif  // __DKNB_COMMON_SINGLETON_HPP__
