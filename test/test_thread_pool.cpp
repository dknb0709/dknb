#include <future>
#include <iostream>
#include <parallel/thread/thread_pool.hpp>
#include <vector>

int square(int n) { return n * n; }

void test_use_directly() {
  std::vector<std::future<int>> results;
  {
    dknb::parallel::thread_pool pool(5);
    for (int i = 0; i < 8; ++i) {
      results.push_back(pool.enqueue(square, static_cast<int>(i)));
    }

    for (auto &result : results) {
      result.wait();
      std::cout << result.get() << ' ';
    }
    std::cout << std::endl;
  }
}

class test_composition {
  std::vector<std::future<int>> results_;
  std::shared_ptr<dknb::parallel::thread_pool> pool_;
  struct worker_process {
    int operator()(int n) { return n * n; }
  };

 public:
  test_composition(int threads)
      : pool_(new dknb::parallel::thread_pool(threads)) {}
  virtual ~test_composition() {}

  void request(int n) {
    std::function<int()> f = std::bind(test_composition::worker_process(), n);
    results_.push_back(pool_->enqueue(f));
  }

  void get_results() {
    for (auto &result : results_) {
      result.wait();
      std::cout << result.get() << ' ';
    }
    std::cout << std::endl;
  }
};

int main() {
  std::cout << "test_use_directly" << std::endl;
  test_use_directly();

  std::cout << "test_composition" << std::endl;
  test_composition com(3);
  for (int i = 0; i < 8; ++i) {
    com.request(i);
  }
  com.get_results();

  std::cin.get();
}
