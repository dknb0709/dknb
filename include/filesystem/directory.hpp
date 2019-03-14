#ifndef _DKNB_FILESYETEM_DIRECTORY_HPP_
#define _DKNB_FILESYETEM_DIRECTORY_HPP_

#include <filesystem/core/entry.hpp>
#include <filesystem>

namespace dknb {

class Directory : public Entry {
 public:
  Directory(const std::string &name) : Entry(name) {}
};

}  // namespace dknb

#endif  // _DKNB_FILESYETEM_DIRECTORY_HPP_
