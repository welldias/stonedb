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

#include "scheduler.h"

namespace Task {

	Scheduler::Scheduler(std::shared_ptr<Detail::Threadpool> threadpool, size_t maxWorkers)
		: m_threadpool(std::move(threadpool))
		, m_maxWorkers(maxWorkers) {}

	std::future<void> Scheduler::Stop(bool discard) {
		std::lock_guard<std::mutex> guard(m_mutex);

		m_stopped = true;
		m_periodicTasks.clear();
		if (discard) {
			m_tasks.Clear();
		}
		auto task = std::make_shared<std::packaged_task<void()>>([this] {
			std::unique_lock<std::mutex> guard(m_mutex);
			bool isLast = m_workerCount == 0;

			// Guarantee that the task finishes last.
			while (!isLast) {
				guard.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				guard.lock();
				isLast = m_workerCount == 0;
			}
			});
		auto future = task->get_future();
		Detail::Timepoint timepoint = Detail::Clock::now();

		if (!m_tasks.empty()) {
			// Set the task just after the last one.
			timepoint = m_tasks.top().GetTimepoint() + std::chrono::nanoseconds(1);
		}
		auto functor = [task = std::move(task)]() mutable {
			(*task)();
		};
		// Push the task directly into the pool to avoid id collisions.
		m_threadpool->Execute({ std::move(functor), timepoint });

		return future;
	}

	void Scheduler::Remove(const std::string& id) {
		std::lock_guard<std::mutex> guard(m_mutex);

		m_tasks.Erase(TimedTask(m_hasher(id)));
	}

	bool Scheduler::IsScheduled(const std::string& id) const {
		std::lock_guard<std::mutex> guard(m_mutex);
		const size_t hash = m_hasher(id);
		const TimedTask task(hash);

		return m_tasks.Contain(task) || m_periodicTasks.find(hash) != m_periodicTasks.end();
	}

	// Private methods

	void Scheduler::AddTask(size_t hash, std::function<void()> functor, Detail::Timepoint timepoint, bool reschedulable) {
		if (m_stopped) {
			return;
		}
		if (reschedulable) {
			m_periodicTasks[hash] = functor;
		}
		m_tasks.emplace(hash, std::move(functor), timepoint);
		this->ProcessTasks();
	}

	void Scheduler::ProcessTasks() {
		if (m_tasks.empty() || m_workerCount == m_maxWorkers) {
			return;
		}
		auto task = m_tasks.top();
		m_tasks.pop();

		++m_workerCount;
		// NOTE: Use static_cast instead of std::move to avoid slicing warning from clang-tidy.
		m_threadpool->Execute(static_cast<Detail::TimedTask&&>(task));
	}

} /* namespace Task */
