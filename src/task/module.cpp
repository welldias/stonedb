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

#include "module.h"

#include <cassert>

#include "manager.h"
#include "scheduler.h"
#include "thread_pool.h"

namespace Task {
namespace Module {
namespace {

    std::shared_ptr<Detail::Threadpool> m_threadpool;

    } /* namespace */

    void Init(size_t workerCount) {
      assert(!m_threadpool);
      m_threadpool = std::make_shared<Detail::Threadpool>(workerCount);
    }

    std::unique_ptr<Manager> MakeManager(size_t workerCount) {
      assert(m_threadpool);
      return std::make_unique<Manager>(m_threadpool, workerCount);
    }

    std::unique_ptr<Scheduler> MakeScheduler(size_t workerCount) {
      assert(m_threadpool);
      return std::make_unique<Scheduler>(m_threadpool, workerCount);
    }

} /* namespace Module */
} /* namespace Task */
