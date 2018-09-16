#include "thread_pool.hpp"

namespace dknb {
namespace parallel {

  thread_pool::thread_pool(std::size_t nThreads) : stop(false)
  {
    for (int i = 0; i < nThreads; ++i) {
      workers.push_back(std::thread(worker(*this)));
    }
  }

  thread_pool::~thread_pool()
  {
    {
      std::lock_guard<std::mutex> lock(queue_mutex);
      stop = true;
    }
    condition.notify_all();
    for (auto& worker : workers) {
      worker.join();
    }
  }

  worker::worker(thread_pool& tp) : pool(tp)
  { }

  worker::~worker()
  { }

  void worker::operator()()
  {
    while (true)
    {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(pool.queue_mutex);
        pool.condition.wait(lock, [this](){
          return pool.stop || !pool.tasks.empty();
        });

        if (pool.stop) return;

        task = std::move(pool.tasks.front());
        pool.tasks.pop_front();
      }
      task();
    }
  }

} // parallel
} // dknb
