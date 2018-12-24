#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <string>

#include <stdexcept>

enum Event {
  EvSwitchOn = 0,
  EvSwitchOff,
  EvStopped,
  EvStarted
};

class state_base {
public:
  virtual void process_event(int ev) const = 0;
  virtual char const* to_string() const {
    return "[warn] not implemented";
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

class passive : public state<passive> {
public:
  virtual void process_event(int ev) const override {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual char const* to_string() const override {
    return "passive";
  }
};

class active : public state<active> {
public:
  virtual void process_event(int ev) const override {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual char const* to_string() const override {
    return "passive";
  }
};

class stopped : public state<stopped> {
public:
  virtual void process_event(int ev) const override {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual char const* to_string() const override {
    return "stopped";
  }
};

class started : public state<started> {
public:
  virtual void process_event(int ev) const override {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual char const* to_string() const override {
    return "started";
  }
};

class paused : public state<paused> {
public:
  virtual void process_event(int ev) const override {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual char const* to_string() const override {
    return "paused";
  }
};

class transition_table {
  state_base* initial_;
  std::vector<int> events_;
  std::map<state_base*, std::vector<state_base*>> trans_;
  transition_table* parent_;
public:
  transition_table(
      state_base* initial,
      std::vector<int> events,
      std::map<state_base*, std::vector<state_base*>> trans)
      : initial_(initial)
      , events_(events)
      , trans_(trans)
  { }
  state_base* get_initial_state() { return initial_; }
  transition_table* get_parent() { return parent_; }
  state_base* resolve(state_base* from, int ev) {
    return trans_[from][events_[ev]];
  }
  void nested(transition_table* child) {
    child->parent_ = this;
  }
};

// 'delegator' would be used to
// check whether the included
// transition talble is defined
// when the transition is completed
class delegator {
  std::map<state_base*, transition_table*> map_;
public:
  delegator(std::map<state_base*, transition_table*> map)
    : map_(map)
  { }
  transition_table* try_dispatch_child(state_base* new_state) {
    return map_[new_state];
  }
};

class state_machine {
  state_base* current_;
  transition_table* trans_;
  delegator* delegator_;
  state_base* resolve(int ev) {
    state_base* next = trans_->resolve(current_, ev);
    if (next == nullptr) {
      trans_ = trans_->get_parent();
      current_ = trans_->get_initial_state();
      next = current_;
    }
    return next;
  }
  void process_event(int ev) {
    current_->process_event(ev);
  }
public:
  state_machine(state_base* initial, transition_table* trans, delegator* deleg)
    : current_(initial)
    , trans_(trans)
    , delegator_(deleg)
  { }
  
  void dispatch_event(int ev) {
    state_base* prev = current_;
    state_base* next = resolve(ev);
    transition_table* trans = delegator_->try_dispatch_child(next);
    if (trans != nullptr) {
      trans_ = trans;
      current_ = trans->get_initial_state();
    }
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

  state_base* p_passive = passive::get_instance();
  state_base* p_active = active::get_instance();

  state_base* p_stopped = stopped::get_instance();
  state_base* p_started = started::get_instance();
  state_base* p_paused = paused::get_instance();

  transition_table trans_table_root = {
    p_passive, { EvSwitchOff, EvSwitchOn },
    {
      { p_passive, { p_passive, p_active } },
      { p_active, { p_passive, p_active } }
    }
  };

  transition_table trans_table_active = {
    p_stopped, { EvStopped, EvStarted },
    {
      { p_stopped, { p_stopped, p_started } },
      { p_started, { p_stopped, p_paused } },
      { p_paused, { p_stopped, p_started } }
    }
  };

  trans_table_root.nested(&trans_table_active);

  std::map<state_base*, transition_table*> mp;
  mp[p_active] = &trans_table_active;
  delegator deleg(mp);

  state_machine sm(p_passive, &trans_table_active, &deleg);

  std::vector<int> feed = {
    EvSwitchOn, EvStopped, EvStarted, EvSwitchOff
  };

  for (int e : feed) {
    sm.dispatch_event(e);
  }

}

