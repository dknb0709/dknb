#include <iostream>
#include <array>
#include <vector>
#include <map>

enum Events {
  EvStart = 0,
  EvStop
};

enum States {
  Stopped = 0,
  Started,
  Paused
};

template <size_t NumEvents, size_t NumStates>
class state_machine {
  int current_;
  std::array<int, NumEvents> events_;
  std::map<int, std::array<int, NumStates>> trans_;
  int resolve_next(int ev) {
    return trans_[current_][events_[ev]];
  }
  void perform() {
    switch (current_) {
      case Stopped:
        std::cout << "perform Stopped" << std::endl; break;
      case Started:
        std::cout << "perform Started" << std::endl; break;
      case Paused:
        std::cout << "perform Paused" << std::endl; break;
      default:
        break;
    }
  }
public:
  state_machine(
      int initial,
      std::array<int, NumEvents> events,
      std::map<int, std::array<int, NumStates>> trans)
      : current_(initial)
      , events_(events)
      , trans_(trans)
  { }
  void dispatch_event(int ev) {
    int prev = current_;
    current_ = resolve_next(ev);
    if (prev != current_) {
      std::cout << "switch state, from " << prev << " to " << current_ << std::endl;
    }
    perform();
  }
};

int main () {

  state_machine<2, 2> sm(
      Stopped, { EvStart, EvStop },
      {
        { Stopped, { Started, Stopped } },
        { Started, { Paused, Stopped } }
      });

  std::vector<Events> feed = {
    EvStart, EvStop, EvStart, EvStart, EvStop
  };

  for (auto ev : feed) {
    sm.dispatch_event(static_cast<int>(ev));
  }

}

