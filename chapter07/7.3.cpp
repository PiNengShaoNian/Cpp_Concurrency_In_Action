// 代码清单7.3　无锁栈，但这种实现会泄漏节点内存
#include <atomic>
#include <memory>

template <typename T>
class lock_free_stack {
 private:
  struct node {
    std::shared_ptr<T> data;
    node *next;
    node(T const &data_) : data(std::make_shared<T>(data)) {}
  };
  std::atomic<node *> head;

 public:
  void push(T const &value) {
    node *const new_node = new node(value);
    new_node->next = head.load();
    while (!head.compare_exchange_weak(new_node->next, new_node))
      ;
  }

  std::shared_ptr<T> pop() {
    node *old_head = head.load();
    while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
      ;
    return old_head ? old_head->data : std::shared_ptr<T>();
  }
};