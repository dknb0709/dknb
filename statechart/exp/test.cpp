#include <iostream>
#include <vector>
#include <map>

class transition_table {
  int val_;
  std::vector<int> events_;
  std::map<int, std::vector<char const*>> trans_;
public:
  transition_table(
      int initial,
      std::vector<int> events,
      std::map<int, std::vector<char const*>> trans)
      : val_(initial)
      , events_(events)
      , trans_(trans)
  { }
  void dispatch(int event) {
    const char* word = trans_[1][event];
    std::cout << word << std::endl;
  }
};

int main () {
  transition_table table = {
    3, {1, 2},
    {
      { 1, { "one", "ichi" } },
      { 2, { "two", "ni" } },
      { 3, { "three", "san" } }
    }
  };

  table.dispatch(0);
  table.dispatch(1);

}

