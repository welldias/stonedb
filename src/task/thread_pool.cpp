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

#include "thread_pool.h"

namespace Task {
namespace Detail {

	Threadpool::Threadpool(size_t threadCount) {
		m_workers.reserve(threadCount);
		for (size_t i = 0; i < threadCount; ++i) {
			m_workers.emplace_back(&Threadpool::ProcessTasks, this);
		}
	}

	Threadpool::~Threadpool() {
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			m_stopRequested = true;
		}
		m_conditionVariable.notify_all();
		for (auto& worker : m_workers) {
			if (worker.joinable()) {
				worker.join();
			}
		}
	}

	void Threadpool::Execute(TimedTask task) {
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			_tasks.push(std::move(task));
		}
		m_conditionVariable.notify_one();
	}

	void Threadpool::ProcessTasks() {
		bool stopped = false;

		while (!stopped) {
			std::unique_lock<std::mutex> lock(m_mutex);
			auto timeout = Clock::now();
			bool taskReady = false;

			while (!taskReady) {
				taskReady = m_conditionVariable.wait_until(lock, timeout, [this, &timeout, &stopped] { //
					stopped = m_stopRequested;
					if (stopped) {
						return true;
					}
					// The condvar can be unblocked by calling notify_xxx to signal that a new task has been added.
					// We should update the timeout if the new task has a higher or lower priority.
					if (!_tasks.empty()) {
						timeout = _tasks.top().GetTimepoint();
						return timeout <= Clock::now();
					}
					timeout = Clock::now() + std::chrono::hours(1);

					return false;
					});
				if (stopped) {
					return;
				}
			}

			auto task = _tasks.top();
			_tasks.pop();

			lock.unlock();
			task();
		}
	}

} /* namespace Detail */
} /* namespace Task */
