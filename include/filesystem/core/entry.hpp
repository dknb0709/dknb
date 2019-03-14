#ifndef _DKNB_FILESYETEM_CORE_ENTRY_HPP_
#define _DKNB_FILESYETEM_CORE_ENTRY_HPP_

#include <string>

namespace dknb {

class Entry {
  std::string name_;

 public:
  Entry(const std::string& name) : name_(name) {}
  std::string getName() { return name_; }
};

}  // namespace dknb

#endif  // _DKNB_FILESYETEM_CORE_ENTRY_HPP_
