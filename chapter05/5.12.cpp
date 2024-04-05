// 代码清单5.12　栅栏可以令宽松操作服从一定的次序
#include <assert.h>

#include <atomic>
#include <thread>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y() {
  x.store(true, std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_release);
  y.store(true, std::memory_order_relaxed);
}

void read_y_then_x() {
  while (!y.load(std::memory_order_relaxed))
    ;
  std::atomic_thread_fence(std::memory_order_acquire);
  if (x.load(std::memory_order_relaxed)) {
    ++z;
  }
}

int main() {
  x = false;
  y = false;
  z = 0;
  std::thread a(write_x_then_y);
  std::thread b(read_y_then_x);
  a.join();
  b.join();
  assert(z.load() != 0);
}