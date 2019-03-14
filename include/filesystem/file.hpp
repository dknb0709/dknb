#ifndef _DKNB_FILESYETEM_FILE_HPP_
#define _DKNB_FILESYETEM_FILE_HPP_

#include <filesystem/core/entry.hpp>

namespace dknb {

class File : public Entry {
 public:
  File(const std::string &name) : Entry(name) {}
};

}  // namespace dknb

#endif  // _DKNB_FILESYETEM_FILE_HPP_
