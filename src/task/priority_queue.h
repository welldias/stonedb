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

#ifndef __PROJECT_STONE_TASK_PRIORITYQUEUE_H__
#define __PROJECT_STONE_TASK_PRIORITYQUEUE_H__

#include <algorithm>
#include <queue>
#include <vector>

namespace Task {
namespace Detail {

    template <typename T, typename Comp = std::less<>>
    class PriorityQueue : public std::priority_queue<T, std::vector<T>, Comp> {
     public:
      bool Erase(const T& e) {
        const auto it = std::find(this->c.begin(), this->c.end(), e);

        if (it == this->c.end()) {
          return false;
        }
        this->c.erase(it);
        std::make_heap(this->c.begin(), this->c.end(), this->comp);

        return true;
      }

      bool Update(const T& e) {
        const auto it = std::find(this->c.begin(), this->c.end(), e);

        if (it == this->c.end()) {
          return false;
        }
        *it = e;
        std::make_heap(this->c.begin(), this->c.end(), this->comp);

        return true;
      }

      bool Contain(const T& e) const {
        const auto it = std::find(this->c.begin(), this->c.end(), e);

        return it != this->c.end();
      }

      void Clear() {
        this->c.clear();
      }
    };

} /* namespace Detail */
} /* namespace Task */

#endif /* __PROJECT_STONE_TASK_PRIORITYQUEUE_H__ */
