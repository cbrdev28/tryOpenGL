#ifndef THREAD_POOL_MANAGER_H_
#define THREAD_POOL_MANAGER_H_

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

struct ThreadTask {
  std::promise<void> promise;
  std::function<void(std::promise<void>)> function;
};

class ThreadPoolManager {
 public:
  ThreadPoolManager();
  ~ThreadPoolManager();

  ThreadPoolManager(const ThreadPoolManager &) = delete;
  ThreadPoolManager(ThreadPoolManager &&) noexcept = delete;
  auto operator=(const ThreadPoolManager &) -> ThreadPoolManager & = delete;
  auto operator=(ThreadPoolManager &&) noexcept -> ThreadPoolManager & = delete;

  inline auto getThreadCount() -> unsigned int { return threads_.size(); }
  auto queueTask(std::function<void(std::promise<void>)> &&function) -> std::future<void>;

 private:
  bool shutdown_{false};
  std::vector<std::thread> threads_{};
  std::mutex threadMutex_;
  std::condition_variable threadCondition_;
  std::queue<ThreadTask> threadTasks_;

  void threadFunction();
};

#endif