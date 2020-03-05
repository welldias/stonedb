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

#ifndef __PROJECT_STONE_TASK_SCHEDULER_H__
#define __PROJECT_STONE_TASK_SCHEDULER_H__

#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>

#include "priority_queue.h"
#include "task.h"
#include "thread_pool.h"

namespace Task {

	using Duration = std::chrono::nanoseconds;

	//! The task scheduler.
	class Scheduler {
	public:
		//! Task scheduler constructor.
		//! @param threadpool The threadpool owning the workers.
		//! @param maxWorkers The maximum number of parallel executions.
		Scheduler(std::shared_ptr<Detail::Threadpool> threadpool, size_t maxWorkers);

		//! Synchronize and stop the task scheduler.
		//! @param discard @c true Discard the tasks scheduled for the future.
		//! @return A future that signals when the scheduler can be destroyed.
		std::future<void> Stop(bool discard = false);

		//! Add a new task to the scheduler.
		//! @param id The unique identity of the task.
		//! @param delay The delay to wait before executing the task.
		//! @param function The function to execute.
		//! @param args the parameters to pass to the function.
		template <class F, class... Args>
		auto ScheduleIn(const std::string& id, Duration delay, F&& function, Args&&... args) {
			return this->scheduleAt(id, Detail::Clock::now() + delay, std::forward<F>(function), std::forward<Args>(args)...);
		}

		//! Add a new task to the scheduler.
		//! @param id The unique identity of the task.
		//! @param timepoint The timepoint to reach before executing the task.
		//! @param function The function to execute.
		//! @param args the parameters to pass to the function.
		template <class F, class... Args>
		auto ScheduleAt(const std::string& id, Detail::Timepoint timepoint, F&& function, Args&&... args) //
			-> std::future<typename std::result_of<F(Args...)>::type> {
			using ReturnType = typename std::result_of<F(Args...)>::type;
			std::future<ReturnType> future;

			auto task = std::make_shared<std::packaged_task<ReturnType()>>( //
				std::bind(std::forward<F>(function), std::forward<Args>(args)...));
			future = task->get_future();

			auto functor = [this, task = std::move(task)]() {
				(*task)();
				{
					std::lock_guard<std::mutex> guard(m_mutex);

					--m_workerCount;
					this->processTasks();
				}
			};
			std::lock_guard<std::mutex> guard(m_mutex);
			this->addTask(m_hasher(id), std::move(functor), timepoint);

			return future;
		}

		//! Add a new periodic task to the scheduler.
		//! @param id The unique identity of the task.
		//! @param delay The minimum duration separating two executions.
		//! @param function The function to execute.
		//! @param args the parameters to pass to the function.
		template <class F, class... Args>
		void ScheduleEvery(const std::string& id, Duration delay, F&& function, Args&&... args) {
			auto task = std::bind(std::forward<F>(function), std::forward<Args>(args)...);
			const auto hash = m_hasher(id);

			auto periodicTask = [this, hash, task = std::move(task), delay]() mutable {
				task();

				{
					std::lock_guard<std::mutex> guard(m_mutex);

					--m_workerCount;
					auto periodicTask = m_periodicTasks.find(hash);
					if (periodicTask == m_periodicTasks.end()) {
						return;
					}
					this->addTask(hash, periodicTask->second, Detail::Clock::now() + delay);
				}
			};
			std::lock_guard<std::mutex> guard(m_mutex);
			this->addTask(hash, std::move(periodicTask), Detail::Clock::now() + delay, true);
		}

		//! Remove a task from the scheduler.
		//! @param id The unique identity of the task.
		void Remove(const std::string& id);

		//! Check if a task is scheduled.
		//! @param id The unique identity of the task.
		//! @return @c true if it is scheduled, @c false otherwise.
		bool IsScheduled(const std::string& id) const;

	private:
		class TimedTask : public Detail::TimedTask {
		public:
			explicit TimedTask(size_t id)
				: Detail::TimedTask(nullptr, {})
				, m_id(id) {}
			TimedTask(size_t id, Detail::Task functor, Detail::Timepoint timepoint)
				: Detail::TimedTask(std::move(functor), timepoint)
				, m_id(id) {}

			bool operator==(const TimedTask& other) const {
				return m_id == other.m_id;
			}

		private:
			size_t m_id;
		};

	private:
		void AddTask(size_t hash, std::function<void()> functor, Detail::Timepoint timepoint, bool reschedulable = false);
		void ProcessTasks();

	private:
		std::shared_ptr<Detail::Threadpool> m_threadpool;
		Detail::PriorityQueue<TimedTask, std::greater<>> m_tasks;
		std::unordered_map<size_t, std::function<void()>> m_periodicTasks;
		std::hash<std::string> m_hasher;
		mutable std::mutex m_mutex;
		size_t m_maxWorkers;
		size_t m_workerCount{ 0 };
		bool m_stopped{ false };
	};

} /* namespace Task */

#endif /* __PROJECT_STONE_TASK_SCHEDULER_H__ */
