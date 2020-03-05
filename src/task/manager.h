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


#ifndef __PROJECT_STONE_TASK_MANAGER_H__
#define __PROJECT_STONE_TASK_MANAGER_H__

#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <utility>

#include "task.h"
#include "thread_pool.h"

namespace Task {

	//! The task manager.
	class Manager {
	public:
		//! Task manager constructor.
		//! @param threadpool The threadpool owning the workers.
		//! @param maxWorkers The maximum number of parallel executions.
		Manager(std::shared_ptr<Detail::Threadpool> threadpool, size_t maxWorkers);

		//! Synchronize and stop the task manager.
		//! @return A future that signals when the manager can be destroyed.
		std::future<void> Stop();

		//! Add a new task to the manager.
		//! @param function The function to execute.
		//! @param args the parameters to pass to the function.
		template <class F, class... Args>
		auto Push(F&& function, Args&&... args) //
			-> std::future<typename std::result_of<F(Args...)>::type> {
			using ReturnType = typename std::result_of<F(Args...)>::type;

			auto task = std::make_shared<std::packaged_task<ReturnType()>>( //
				std::bind(std::forward<F>(function), std::forward<Args>(args)...));
			auto future = task->get_future();

			auto functor = [this, task = std::move(task)]() mutable {
				(*task)();
				{
					std::lock_guard<std::mutex> guard(m_mutex);

					--m_workerCount;
					this->ProcessTasks();
				}
			};
			this->AddTask(std::move(functor));

			return future;
		}

	private:
		void AddTask(std::function<void()> functor);
		void ProcessTasks();

	private:
		std::shared_ptr<Detail::Threadpool> m_threadpool;
		std::queue<Detail::TimedTask> m_tasks;
		std::mutex m_mutex;
		size_t m_maxWorkers;
		size_t m_workerCount{ 0 };
		bool m_stopped{ false };
	};

} /* namespace Task */

#endif /*__PROJECT_STONE_TASK_MANAGER_H__*/
