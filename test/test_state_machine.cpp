#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <string>

#include "../state_machine.hpp"

enum Event {
  EvStart = 0,
  EvStop
};

class stopped : public state<stopped> {
  friend class state<stopped>;
  stopped(){}
public:
  virtual void process_event(int ev) override {
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
  virtual void process_event(int ev) override {
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
  virtual void process_event(int ev) override {
    std::cout << "process " << to_string() << std::endl;
  }
  virtual char const* to_string() const override {
    return "paused";
  }
};

int main () {

  state_base* Stopped = stopped::get_instance();
  state_base* Started = started::get_instance();
  state_base* Paused = paused::get_instance();

  state_machine<2> sm(
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

