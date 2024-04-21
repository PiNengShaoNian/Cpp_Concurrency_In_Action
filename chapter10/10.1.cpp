// 代码清单10.1　一个具备内部同步功能的类，以及作用在其上的并行算法函数操作
#include <algorithm>
#include <execution>
#include <mutex>

class X {
  mutable std::mutex m;
  int data;

 public:
  X() : data(0) {}
  int get_value() const {
    std::lock_guard guard(m);
    return data;
  }
  int increment() {
    std::lock_guard guard(m);
    ++data;
  }
};

void increment_all(std::vector<X> &v) {
  std::for_each(std::execution::par, v.begin(), v.end(),
                [](X &x) { x.increment(); });
}