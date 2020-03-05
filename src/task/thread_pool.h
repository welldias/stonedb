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

#ifndef __PROJECT_STONE_TASK_THREADPOOL_H__
#define __PROJECT_STONE_TASK_THREADPOOL_H__

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

		void Execute(TimedTask task);

	private:
		void ProcessTasks();

	private:
		std::priority_queue<TimedTask, std::vector<TimedTask>, std::greater<>> _tasks;
		std::vector<std::thread> m_workers;
		std::condition_variable m_conditionVariable;
		std::mutex m_mutex;
		bool m_stopRequested{ false };
	};

} /* namespace Detail */
} /* namespace Task */

#endif /*__PROJECT_STONE_TASK_THREADPOOL_H__*/
