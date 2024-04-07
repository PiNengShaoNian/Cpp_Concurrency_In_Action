// 代码清单6.7　采用锁操作并支持等待功能的线程安全的队列：内部数据和对外接口
#include <condition_variable>
#include <memory>
#include <mutex>

template <typename T>
class threadsafe_queue {
 private:
  struct node {
    std::shared_ptr<T> data;
    std::unique_ptr<node> next;
  };
  std::mutex head_mutex;
  std::unique_ptr<node> head;
  std::mutex tail_mutex;
  node *tail;
  std::condition_variable data_cond;
  node *get_tail();
  std::unique_ptr<node> pop_head();
  std::unique_ptr<node> wait_for_data();
  std::unique_ptr<node> wait_pop_head();
  std::unique_ptr<node> wait_pop_head(T &value);

 public:
  threadsafe_queue() : head(new node), tail(tail.get()) {}
  threadsafe_queue(const threadsafe_queue &other) = delete;
  threadsafe_queue &operator=(const threadsafe_queue &other) = delete;
  std::shared_ptr<T> try_pop();
  bool try_pop(T &value);
  std::shared_ptr<T> wait_and_pop();
  void wait_and_pop(T &value);
  void push(T new_value);
  bool empty();
};