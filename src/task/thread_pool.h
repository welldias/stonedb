#ifndef TASK_MANAGER_DETAIL_THREADPOOL_HPP
#define TASK_MANAGER_DETAIL_THREADPOOL_HPP

#include <algorithm>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "task.h"

namespace Task {
namespace Detail {

class Threadpool {
 public:
  explicit Threadpool(size_t threadCount);
  ~Threadpool();

  void execute(TimedTask task);

 private:
  void processTasks();

 private:
  std::priority_queue<TimedTask, std::vector<TimedTask>, std::greater<>> _tasks;
  std::vector<std::thread> _workers;
  std::condition_variable _cv;
  std::mutex _mutex;
  bool _stopRequested{ false };
};

} /* namespace Detail */
} /* namespace Task */

#endif
