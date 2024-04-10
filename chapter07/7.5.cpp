// 代码清单7.5　采取引用计数的内存释放机制
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
  std::atomic<node *> to_be_deleted;
  std::atomic<node *> head;
  std::atomic<unsigned> threads_in_pop;

  static void delete_nodes(node *nodes) {
    while (nodes) {
      node *next = nodes->next;
      delete nodes;
      nodes = next;
    }
  }
  void try_reclaim(node *old_head) {
    if (threads_in_pop == 1) {
      node *nodes_to_delete = to_be_deleted.exchange(nullptr);
      if (!--threads_in_pop) {
        // pop仅被当前线程调用
        delete_nodes(nodes_to_delete);
      } else if (nodes_to_delete) {
        chain_pending_nodes(nodes_to_delete);
      }
      delete old_head;
    } else {
      chain_pending_node(old_head);
      --threads_in_pop;
    }
  }
  void chain_pending_nodes(node *nodes) {
    node *last = nodes;
    while (node *next = last->next) {
      last = next;
    }
    chain_pending_nodes(nodes, last);
  }
  void chain_pending_nodes(node *first, node *last) {
    last->next = to_be_deleted;
    while (!to_be_deleted.compare_exchange_weak(last->next, first))
      ;
  }
  void chain_pending_node(node *n) { chain_pending_nodes(n, n); }
};