# ifndef DKNB_PARALLEL_THREAD_POOL
# define DKNB_PARALLEL_THREAD_POOL

#include <thread>
#include <functional>
#include <memory>
#include <future>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>

#include <iostream>

namespace dknb {
namespace parallel {

  class worker;

  class thread_pool {
  public:
    thread_pool(size_t nThreads);
    ~thread_pool();

    template <typename F, typename ... Args>
    auto enqueue(F&& f, Args&& ... args)
    -> std::future<typename std::result_of<F(Args...)>::type>
    {
      typedef typename std::result_of<F(Args...)>::type result_type;
      auto task = std::make_shared<std::packaged_task<result_type(Args...)>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
      );
      std::future<result_type> res = task->get_future();
      {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace_back(
          [task, args...](){
            (*task)(args...);
          }
        );
      }
      condition.notify_one();
      return res;
    }

  private:
    friend class worker;

    std::vector<std::thread> workers;
    std::deque<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
  };

  class worker {
  public:
    worker(thread_pool& pool);
    ~worker();
    void operator()();
  private:
    thread_pool& pool;
  };

} // parallel
} // dknb

# endif // DKNB_PARALLEL_THREAD_POOL
