#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace andeme {

template <class T>
class ThreadSafeQueue {
 public:
  T pop() {
    std::unique_lock<std::mutex> lock(mutex_);

    cond_.wait(lock, [this]() { return !queue_.empty(); });

    auto value = std::move(queue_.front());
    queue_.pop();
    return value;
  }

  template <class U> void push(U &&value) {
    {
      std::lock_guard lock(mutex_);
      queue_.push(std::forward<U>(value));
    }
    cond_.notify_one();
  }

 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

}  // namespace andeme
