#ifndef _DKNB_PROPERTY_HPP_
#define _DKNB_PROPERTY_HPP_

#include <fstream>
#include <iostream>
#include <mutex>
#include <regex>
#include <vector>

#include <common/singleton.hpp>

namespace dknb {

struct property : public singleton<property> {
  struct pair {
    std::string key, value;
    std::string to_str() { return value; }
    int to_int() { return std::stoi(value); }
    void parse(const std::string&& s) {
      int trim_starts;
      std::string trimmed = s;
      if ((trim_starts = s.find("#")) != -1) {
        trimmed = s.substr(0, trim_starts);
      }
      size_t idx = trimmed.find("=");
      if (idx != -1 && idx + 1 < s.size()) {
        key = trimmed.substr(0, idx);
        value = trimmed.substr(idx + 1, s.size());
      }
    }
  };

  static void regist(const std::string& prop) { props_[prop] = "registered"; }

  static void load(const std::string& prop) {
    std::unique_lock<std::mutex> lock(io_mtx_);
    std::ifstream ifs(prop + ".properties", std::ios::in);
    if (!ifs.is_open()) {
      std::cerr << "[error] can't open a file: \"" << prop << ".properties\"" << std::endl;
      return;
    }
    regist(prop);
    std::string line;
    while (ifs.is_open() && !ifs.eof()) {
      getline(ifs, line);
      pair p;
      p.parse(std::move(line));
      if (p.key != "" && p.value != "") props_[prop + "::" + p.key] = p.value;
    }
  }

  static pair get(const std::string& key) {
    std::string prop = key.substr(0, key.find("::"));
    if (props_[prop] != "registered") {
      load(prop);
    }
    return {key, props_[key]};
  }

  friend std::ostream& operator<<(std::ostream& out, const property& prop);

  static std::mutex io_mtx_;
  static std::map<std::string, std::string> props_;
};

std::ostream& operator<<(std::ostream& out, const property& prop) {
  for (const auto& [key, value] : prop.props_) {
    out << key << " => " << value << "\n";
  }
  return out;
}

std::mutex property::io_mtx_;
std::map<std::string, std::string> property::props_;

}  // namespace dknb

#endif  // _DKNB_PROPERTY_HPP_
