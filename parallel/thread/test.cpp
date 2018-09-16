#include "thread_pool.hpp"
#include <iostream>
#include <future>
#include <vector>

int square(int n) { return n * n; }

int main () {

  std::vector<std::future<int>> results;
  {
    dknb::parallel::thread_pool pool(5);
    for (int i = 0; i < 8; ++i) {
      results.push_back(pool.enqueue(square, static_cast<int>(i)));
    }

    for (auto& result : results) {
      result.wait();
      std::cout << result.get() << ' ';
    }
  }

  std::cin.get();
}
