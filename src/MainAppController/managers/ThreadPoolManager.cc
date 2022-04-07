#include "cbrpch.h"

#include "ThreadPoolManager.h"

#include <fmt/core.h>

ThreadPoolManager::ThreadPoolManager() {
  auto maxThreads = std::thread::hardware_concurrency();
  fmt::print("Reserving {} threads\n", maxThreads);
  threads_.reserve(maxThreads);
  for (unsigned int i = 0; i < maxThreads; i++) {
    threads_.emplace_back(&ThreadPoolManager::threadFunction, this);
  }
}

ThreadPoolManager::~ThreadPoolManager() {
  {
    std::unique_lock lock(threadMutex_);
    shutdown_ = true;
  }
  threadCondition_.notify_all();

  for (auto& thread : threads_) {
    thread.join();
  }
}

auto ThreadPoolManager::queueTask(std::function<void(std::promise<void>)>&& function) -> std::future<void> {
  std::promise<void> promise;
  std::future<void> future = promise.get_future();
  {
    std::unique_lock lock(threadMutex_);
    threadTasks_.push(ThreadTask{std::move(promise), std::move(function)});
  }
  threadCondition_.notify_one();
  return future;
}

void ThreadPoolManager::threadFunction() {
  for (;;) {
    // Prepare a function/task & promise
    std::function<void(std::promise<void>)> function;
    std::promise<void> promise;
    {
      // Wait on condition
      std::unique_lock lock{threadMutex_};
      threadCondition_.wait(lock, [this]() {
        // Wait until shutdown is true OR a task has been added to the queue
        return (shutdown_ || !threadTasks_.empty());
      });

      if (shutdown_) {
        // Time to shutdown, so break out of the for loop
        break;
      }

      // Take a task from the queue
      ThreadTask aTask = std::move(threadTasks_.front());
      function = std::move(aTask.function);
      promise = std::move(aTask.promise);
      threadTasks_.pop();
    }
    // Run task
    function(std::move(promise));
  }
}
