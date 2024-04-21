// 代码清单10.2　没有内部同步功能的类，以及作用在其上的并行算法函数操作
#include <execution>
#include <mutex>
#include <vector>

class Y {
  int data;

 public:
  Y() : data(0) {}
  int get_value() const { return data; }
  void increment() { ++data; }
}

;
class ProtectedY {
  std::mutex m;
  std::vector<Y> v;

 public:
  void lock() { m.lock(); }
  void unlock() { m.unlock(); }
  std::vector<Y> &get_vec() { return v; }
};

void increment_all(ProtectedY &data) {
  std::lock_guard guard(data);
  auto &v = data.get_vec();
  std::for_each(std::execution::par_unseq, v.begin(), v.end(),
                [](Y &y) { y.increment(); });
}