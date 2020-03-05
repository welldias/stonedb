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

#ifndef __PROJECT_STONE_TASK_TASK_MANAGER_H__
#define __PROJECT_STONE_TASK_TASK_MANAGER_H__

#include <chrono>
#include <functional>
#include <utility>

namespace Task {
namespace Detail {

	using Task = std::function<void()>;
	using Clock = std::chrono::steady_clock;
	using Timepoint = Clock::time_point;

	class TimedTask {
	public:
		TimedTask(Task functor, const Timepoint& timepoint)
			: m_functor(std::move(functor))
			, m_timepoint(timepoint) {}

		const Timepoint& GetTimepoint() const {
			return m_timepoint;
		}

		bool operator>(const TimedTask& other) const {
			return m_timepoint > other.m_timepoint;
		}
		void operator()() {
			m_functor();
		}

	private:
		Task m_functor;
		Timepoint m_timepoint;
	};

} /* namespace Detail */
} /* namespace Task */

#endif /*__PROJECT_STONE_TASK_TASK_MANAGER_H__*/
