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

#ifndef __PROJECT_STONE_TASK_MODULE_H__
#define __PROJECT_STONE_TASK_MODULE_H__

#include <cstddef>
#include <memory>

namespace Task {

// Forward declarations
class Manager;
class Scheduler;

namespace Module {

	//! Initialize the task module.
	//! It creates the threadpool instance used by the managers and schedulers.
	//! @param workerCount The number of workers managed in the pool.
	void Init(size_t workerCount);

	//! Factory function that creates a new task manager.
	//! @param workerCount The maximum number of parallel executions.
	//! Setting one worker will make the manager serialize the tasks execution (cf strand from boost asio).
	//! @return A new task manager.
	std::unique_ptr<Manager> MakeManager(size_t workerCount);

	//! Factory function that creates a new task scheduler.
	//! @param workerCount The maximum number of parallel executions.
	//! @return A new task scheduler.
	std::unique_ptr<Scheduler> MakeScheduler(size_t workerCount);

} /* namespace Module */
} /* namespace Task */

#endif /*__PROJECT_STONE_TASK_MODULE_H__*/
