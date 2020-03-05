/*
This code e based on work of Luc Sinet
For more information please check his Github: https://github.com/Tastyep/TaskManager

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "manager.h"

namespace Task {

Manager::Manager(std::shared_ptr<Detail::Threadpool> threadpool, size_t maxWorkers)
  : m_threadpool(std::move(threadpool))
  , m_maxWorkers(maxWorkers) {}

std::future<void> Manager::Stop() {
  auto task = std::make_shared<std::packaged_task<void()>>([this] {
    std::unique_lock<std::mutex> guard(m_mutex);
    bool isLast = m_workerCount == 1;

    // Guarantee that the task finishes last.
    while (!isLast) {
      guard.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      guard.lock();
      isLast = m_workerCount == 1;
    }
  });
  auto future = task->get_future();

  // Adding a new task and expecting the future guarantees that the last batch of tasks is being executed.
  auto functor = [task = std::move(task)]() mutable {
    (*task)();
  };
  std::lock_guard<std::mutex> guard(m_mutex);

  m_stopped = true;
  m_tasks.emplace(std::move(functor), Detail::Clock::now());
  this->ProcessTasks();

  return future;
}

void Manager::AddTask(std::function<void()> functor) {
  std::lock_guard<std::mutex> guard(m_mutex);

  if (m_stopped) {
    return;
  }
  m_tasks.emplace(std::move(functor), Detail::Clock::now());
  this->ProcessTasks();
}

void Manager::ProcessTasks() {
  if (m_tasks.empty() || m_workerCount == m_maxWorkers) {
    return;
  }
  auto task = std::move(m_tasks.front());
  m_tasks.pop();

  ++m_workerCount;
  m_threadpool->Execute(std::move(task));
}

} /* namespace Task */
