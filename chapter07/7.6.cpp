// 代码清单7.6　采用风险指针实现的pop()函数
#include <atomic>
#include <memory>

template <typename T>
std::shared_ptr<T> pop() {
  std::atomic<void *> &hp = get_hazard_pointer_for_current_thread();
  node *old_head = head.load();
  do {
    node *temp;
    do {
      temp = old_head;
      hp.store(old_head);
      old_head = head.load();
    } while (old_head != temp);
  } while (old_head && !head.compare_exchange_strong(old_head, old_head->next));
  hp.store(nullptr);  // 一旦更新了head指针，便将风险指针清零
  std::shared_ptr<T> res;
  if (old_head) {
    res.swap(old_head->data);
    // 删除节点之前先判断它是否被风险指针涉及
    if (outstanding_hazard_pointers_for(old_head)) {
      reclaim_later(old_head);
    } else {
      delete old_head;
    }
    delete_nodes_with_no_hazards();
  }
  return res;
}