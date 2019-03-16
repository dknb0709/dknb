#ifndef _DKNB_FILE_DIRECTORY_HPP_
#define _DKNB_FILE_DIRECTORY_HPP_

#include <common/ReadyState.hpp>
#include <file/core/Entry.hpp>
#include <vector>

namespace dknb {

class Directory : public Entry {
  ReadyState state_;
  std::vector<Entry *> contents_;

 public:
  Directory(const std::string &name) : Entry{name}, state_{ReadyState::GOOD} {
    std::error_code ec;
    if (!std::filesystem::is_directory({name}, ec)) {
      state_ = ReadyState::BAD;
    }
  }

 private:
  bool create(const std::string &prefix) override {
    bool result = true;
    std::filesystem::path path{prefix + "/" + name_};
    result &= std::filesystem::create_directory(path);
    for (auto *entry : contents_) {
      result &= entry->create(path);
    }
    return result;
  }
  bool remove(const std::string &prefix) override {
    bool result = true;
    std::filesystem::path path{prefix + "/" + name_};
    for (auto *entry : contents_) {
      result &= entry->remove(path);
    }
    return result & std::filesystem::remove(path);
  }

 public:
  Directory &add(Entry *entry) {
    contents_.push_back(entry);
    return *this;
  }
  bool create() { return create("."); }
  bool remove() { return remove("."); }
};

}  // namespace dknb

#endif  // _DKNB_FILE_DIRECTORY_HPP_
