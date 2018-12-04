#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <string>

// TODO: refactor this for easy reuse

enum Event {
  EvStart = 0,
  EvStop
};

class state_base {
public:
  virtual void process_event(Event ev) = 0;
  virtual char const* to_string() const {
    return "[warn]not implemented";
  }
};

template <class Derived>
class state : public state_base {
public:
  static Derived* get_instance() {
    static Derived instance;
    return &instance;
  }
};

class stopped : public state<stopped> {
  friend class state<stopped>;
  stopped(){}
public:
  virtual void process_event(Event ev) override {
    std::cout << "process " << to_string() << std::endl;
  }
  virtual char const* to_string() const override {
    return "stopped";
  }
};

class started : public state<started> {
  friend class state<started>;
  started(){}
public:
  virtual void process_event(Event ev) override {
    std::cout << "process " << to_string() << std::endl;
  }
  virtual char const* to_string() const override {
    return "started";
  }
};

class paused : public state<paused> {
  friend class state<paused>;
  paused(){}
public:
  virtual void process_event(Event ev) override {
    std::cout << "process " << to_string() << std::endl;
  }
  virtual char const* to_string() const override {
    return "paused";
  }
};

template <size_t NumEvents, size_t NumStates>
class state_machine {

  state_base* current_;
  
  /**
   * A hierarchy should be constructed with the
   * following two variables as a unit
   * so that it can deal with adding
   * states such as power on/off
   **/
  std::array<int, NumEvents> events_;
  std::map<state_base*, std::array<state_base*, NumEvents>> trans_;

  state_base* resolve_next(int ev) {
    return trans_[current_][events_[ev]];
  }
  void process_event(Event ev) {
    current_->process_event(ev);
  }
public:
  state_machine(
      state_base* initial,
      std::array<int, NumEvents> events,
      std::map<state_base*, std::array<state_base*, NumEvents>> trans)
      : current_(initial)
      , events_(events)
      , trans_(trans)
  { }
  void dispatch_event(Event ev) {
    state_base* prev = current_;
    current_ = resolve_next(ev);
    if (prev != current_) {
      std::cout << "switch state: "
                << prev->to_string()
                << " => "
                << current_->to_string()
                << std::endl;
    }
    process_event(ev);
  }
};

int main () {

  state_base* Stopped = stopped::get_instance();
  state_base* Started = started::get_instance();
  state_base* Paused = paused::get_instance();

  state_machine<2, 3> sm(
      Stopped, { EvStart, EvStop },
      {
        { Stopped, { Started, Stopped } },
        { Started, { Paused, Stopped } },
        { Paused, { Started, Stopped } }
      });

  std::vector<Event> feed = {
    EvStart, EvStop, EvStart, EvStart, EvStop
  };

  for (auto ev : feed) {
    sm.dispatch_event(ev);
  }

}

