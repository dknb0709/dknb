#ifndef _DKNB_FILE_CORE_ENTRY_HPP_
#define _DKNB_FILE_CORE_ENTRY_HPP_

#include <filesystem>
#include <string>

namespace dknb {

class Entry {
protected:
  std::string name_;

 public:
  Entry(const std::string& name) : name_(name) {}
  std::string getName() { return name_; }
  virtual bool create(const std::string &prefix) = 0;
  virtual bool remove(const std::string &prefix) = 0;
};

}  // namespace dknb

#endif  // _DKNB_FILE_CORE_ENTRY_HPP_
