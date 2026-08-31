#include "Infrastructure/ThreadPool.hpp"

ThreadPool::ThreadPool(std::size_t threadCount) {
  const std::size_t workerCount = std::max<std::size_t>(1, threadCount);
  m_workers.reserve(workerCount);

  // Create N workers to process tasks in parallel without blocking the network loop.
  for (std::size_t i = 0; i < workerCount; ++i) {
    m_workers.emplace_back([this] { workerLoop(); });
  }
}

ThreadPool::~ThreadPool() { shutdown(); }

void ThreadPool::enqueue(std::function<void()> task) {
  if (!task) {
    return;
  }

  {
    std::lock_guard lock(m_mutex);
    if (m_stop) {
      return;
    }
    m_tasks.emplace(std::move(task));
  }

  m_condition.notify_one();
}

void ThreadPool::workerLoop() {
  while (true) {
    std::function<void()> task;

    {
      std::unique_lock lock(m_mutex);
      m_condition.wait(lock, [this] { return m_stop || !m_tasks.empty(); });

      if (m_stop && m_tasks.empty()) {
        return;
      }

      task = std::move(m_tasks.front());
      m_tasks.pop();
    }

    task();
  }
}

void ThreadPool::shutdown() {
  {
    std::lock_guard lock(m_mutex);
    m_stop = true;
  }

  m_condition.notify_all();

  for (auto &worker : m_workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }

  m_workers.clear();
}