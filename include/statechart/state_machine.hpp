#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// TODO: refactor this for easy reuse

class state_base {
public:
  virtual void process_event(int ev) = 0;
  virtual char const *to_string() const { return "[warn]not implemented"; }
};

template <class Derived> class state : public state_base {
public:
  static Derived *get_instance() {
    static Derived instance;
    return &instance;
  }
};

template <size_t NumEvents> class state_machine {

  state_base *current_;

  /**
   * A hierarchy should be constructed with the
   * following two variables as a unit
   * so that it can deal with adding
   * states such as power on/off
   **/
  std::array<int, NumEvents> events_;
  std::map<state_base *, std::array<state_base *, NumEvents>> trans_;

  state_base *resolve_next(int ev) { return trans_[current_][events_[ev]]; }
  void process_event(int ev) { current_->process_event(ev); }

public:
  state_machine(
      state_base *initial, std::array<int, NumEvents> events,
      std::map<state_base *, std::array<state_base *, NumEvents>> trans)
      : current_(initial), events_(events), trans_(trans) {}
  void dispatch_event(int ev) {
    state_base *prev = current_;
    current_ = resolve_next(ev);
    if (prev != current_) {
      std::cout << "switch state: " << prev->to_string() << " => "
                << current_->to_string() << std::endl;
    }
    process_event(ev);
  }
};
