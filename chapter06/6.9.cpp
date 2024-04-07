// 代码清单6.9　采用锁操作并支持等待功能的线程安全的队列：wait_and_pop()
#include "6.7.cpp"

template <typename T>
threadsafe_queue<T>::node* threadsafe_queue<T>::get_tail() {
  std::lock_guard<std::mutex> tail_lock(tail_mutex);
  return tail;
}

template <typename T>
std::unique_ptr<typename threadsafe_queue<T>::node>
threadsafe_queue<T>::pop_head() {
  std::unique_ptr<node> old_head = std::move(head);
  head = std::move(old_head->next);
  return old_head;
}

template <typename T>
std::unique_ptr<typename threadsafe_queue<T>::node>
threadsafe_queue<T>::wait_for_data() {
  std::unique_lock<std::mutex> head_lock(head_mutex);
  data_cond.wait(head_lock, [&] { return head.get() != get_tail(); });
  return std::move(head_lock);
}

template <typename T>
std::unique_ptr<typename threadsafe_queue<T>::node>
threadsafe_queue<T>::wait_pop_head() {
  std::unique_lock<std::mutex> head_lock(wait_for_data());
  return pop_head();
}

template <typename T>
std::unique_ptr<typename threadsafe_queue<T>::node>
threadsafe_queue<T>::wait_pop_head(T& value) {
  std::unique_lock<std::mutex> head_lock(wait_for_data());
  value = std::move(*head->data);
  return pop_head();
}

template <typename T>
std::shared_ptr<T> threadsafe_queue<T>::wait_and_pop() {
  std::unique_ptr<node> const old_head = wait_pop_head();
  return old_head->data;
}

template <typename T>
void threadsafe_queue<T>::wait_and_pop(T& value) {
  std::unique_ptr<node> const old_head = wait_pop_head(value);
}