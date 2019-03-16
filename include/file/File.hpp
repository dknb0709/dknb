#ifndef _DKNB_FILE_FILE_HPP_
#define _DKNB_FILE_FILE_HPP_

#include <common/ReadyState.hpp>
#include <file/core/Entry.hpp>
#include <fstream>
#include <vector>

namespace dknb {

class File : public Entry {
  ReadyState state_;
  std::string directory_;

 public:
  File(const std::string &name) : Entry{name}, state_{ReadyState::GOOD} {}
  ~File() {}

 private:
  bool create(const std::string &prefix) override {
    directory_ = prefix;
    bool result = true;
    std::filesystem::path path{prefix + "/" + name_};
    if (state_ == ReadyState::BAD) {
      return false;
    }
    std::ofstream ofs(path);
    return true;
  }
  bool remove(const std::string &prefix) override {
    std::filesystem::path path{prefix + "/" + name_};
    return std::filesystem::remove(path);
  }

 public:
  bool create() { return create("."); }
  bool remove() { return remove("."); }
  std::fstream open(std::ios_base::openmode mode) {
    std::filesystem::path path{directory_ + "/" + name_};
    return std::fstream{path, mode};
  }
};

}  // namespace dknb

#endif  // _DKNB_FILE_FILE_HPP_
