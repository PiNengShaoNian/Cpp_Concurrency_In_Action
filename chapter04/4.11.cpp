// 等待一个具有超时的条件变量
#include <chrono>
#include <condition_variable>
#include <mutex>

std::condition_variable cv;
bool done;
std::mutex m;

bool wait_loop() {
  auto const timeout =
      std::chrono::steady_clock::now() + std::chrono::microseconds(500);
  std::unique_lock lk(m);
  while (!done) {
    if (cv.wait_until(lk, timeout) == std::cv_status::timeout) {
      break;
    }
  }
  return done;
}