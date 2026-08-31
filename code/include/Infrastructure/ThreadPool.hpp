#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

/**
 * @brief Small worker pool used to process client requests asynchronously.
 */
class ThreadPool {
public:
  /**
   * @brief Creates a thread pool with a fixed number of workers.
   * @param threadCount Number of worker threads to start.
   */
  explicit ThreadPool(std::size_t threadCount = std::thread::hardware_concurrency());
  ~ThreadPool();

  /**
   * @brief Queues a task for execution by one worker thread.
   * @param task Function to run in the background.
   */
  void enqueue(std::function<void()> task);

  /**
   * @brief Stops the workers and waits for them to finish.
   */
  void shutdown();

private:
  /**
   * @brief Worker loop that waits for and executes queued tasks.
   */
  void workerLoop();

  std::vector<std::jthread> m_workers;            ///< Threads owned by the pool.
  std::queue<std::function<void()>> m_tasks;      ///< Pending work items.
  std::mutex m_mutex;                             ///< Protects access to queue state.
  std::condition_variable m_condition;            ///< Signals new work or shutdown.
  bool m_stop{false};                             ///< Stops the loop when shutdown starts.
};