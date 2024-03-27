// 一个简单的std::async等价物
#include <future>

template <typename Func, typename... Args>
std::future<typename std::result_of<Func(Args...)>::type> spawn_async(
    Func &&func, Args &&...args) {
  using return_type = typename std::result_of<Func(Args...)>::type;
  std::promise<return_type> promise;
  std::future<return_type> future = promise.get_future();

  std::thread t([&promise, func, args...] {
    try {
      promise.set_value(func(std::forward<Args>(args)...));
    } catch (...) {
      promise.set_exception(std::current_exception());
    }
  });
  t.detach();

  return future;
}