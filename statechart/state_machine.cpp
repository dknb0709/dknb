#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <string>

enum Events {
  EvStart = 0,
  EvStop
};

template <class Derived>
class state_base {
public:
  static Derived* get_instance() {
    static Derived instance;
    return &instance;
  }
  virtual void perform() const = 0;
  virtual char const* to_string() const {
    return "[warn]not implemented";
  }
};

class stopped : public state_base<stopped> {
  stopped(){}
public:
  virtual void perform() const override {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
};

class started : public state_base<started> {
  started(){}
public:
  virtual void perform() const override {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
};

class paused : public state_base<paused> {
  paused(){}
public:
  virtual void perform() const override {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
};

template <size_t NumEvents, size_t NumStates>
class state_machine {
  state_base* current_;
  std::array<int, NumEvents> events_;
  std::map<state_base*, std::array<state_base*, NumStates>> trans_;
  state_base* resolve_next(int ev) {
    return trans_[current_][events_[ev]];
  }
  void perform() {
    current_->perform();
  }
public:
  state_machine(
      int initial,
      std::array<int, NumEvents> events,
      std::map<state_base*, std::array<state_base*, NumStates>> trans)
      : current_(initial)
      , events_(events)
      , trans_(trans)
  { }
  void dispatch_event(int ev) {
    state_base* prev = current_;
    current_ = resolve_next(ev);
    if (prev != current_) {
      std::cout << "switch state, from "
                << prev->to_string()
                << " to " 
                << current_->to_string()
                << std::endl;
    }
    perform();
  }
};

int main () {

  base_state* Stopped = stopped::get_instance();
  base_state* Started = started::get_instance();
  base_state* Stopped = paused::get_instance();

  state_machine<2, 2> sm(
      Stopped, { EvStart, EvStop },
      {
        { Stopped, { Started, Stopped } },
        { Started, { Paused, Stopped } }
        { Paused, { Started, Stopped } }
      });

  std::vector<Events> feed = {
    EvStart, EvStop, EvStart, EvStart, EvStop
  };

  for (auto ev : feed) {
    sm.dispatch_event(static_cast<int>(ev));
  }

}

