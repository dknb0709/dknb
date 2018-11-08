namespace dknb {

template <class T>
struct singleton {
  static T& get_instance() {
    static T instance;
    return instance;
  }
};

}

