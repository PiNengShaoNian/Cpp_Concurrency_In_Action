// 代码清单7.4　若没有线程调用pop()，则趁机删除节点
#include <atomic>
#include <memory>

template <typename T>
class lock_free_stack {
 private:
  std::atomic<unsigned> threads_in_pop;
  struct node {
    std::shared_ptr<T> data;
    node *next;
    node(T const &data_) : data(std::make_shared<T>(data)) {}
  };
  std::atomic<node *> head;

 public:
  std::shared_ptr<T> pop() {
    ++threads_in_pop;
    node *old_head = head.load();
    while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
      ;
    std::shared_ptr<T> res;
    if (old_head) {
      res.swap(old_head->data);
    }
    try_reclaim(old_head);
    return res;
  }
};