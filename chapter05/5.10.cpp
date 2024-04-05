// 代码清单5.10　运用std::memory_order_consume次序同步数据
#include <assert.h>

#include <atomic>
#include <chrono>
#include <string>
#include <thread>

struct X {
  int i;
  std::string s;
};
std::atomic<X *> p;
std::atomic<int> a;
void create_x() {
  X *x = new X;
  x->i = 42;
  x->s = "hello";
  a.store(99, std::memory_order_relaxed);
  p.store(x, std::memory_order_release);
}

void use_x() {
  X *x;
  while (!(x = p.load(std::memory_order_consume))) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  assert(x->i == 42);
  assert(x->s == "hello");
  // 该断言有可能触发，应为其相关操作都被标记为memory_order_relaxed
  // 这也是memory_order_consume和memory_order_acquire不同的地方
  assert(a.load(std::memory_order_relaxed) == 99);
}

int main() {
  std::thread t1(create_x);
  std::thread t2(use_x);
  t1.join();
  t2.join();
}