#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
  explicit ThreadPool(std::size_t threadCount = std::thread::hardware_concurrency());
  ~ThreadPool();

  void enqueue(std::function<void()> task);
  void shutdown();

private:
  void workerLoop();

  std::vector<std::jthread> m_workers;
  std::queue<std::function<void()>> m_tasks;
  std::mutex m_mutex;
  std::condition_variable m_condition;
  bool m_stop{false};
};