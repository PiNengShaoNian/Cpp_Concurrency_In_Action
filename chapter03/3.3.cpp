#include <deque>

// std::stack 容器适配器的接口
template <typename T, typename Container = std::deque<T>>
class stack {
 public:
  explicit stack(const Container &);
  explicit stack(const Container && = Container());

  template <class Alloc>
  explicit stack(const Alloc &);

  template <class Alloc>
  stack(const Container &&, const Alloc &);

  template <class Alloc>
  stack(stack &&, const Alloc &);

  bool empty() const;
  size_t size() const;
  T &top();
  T const &top() const;
  void push(T const &);
  void push(T &&);
  void pop();
  void swap(stack &&);
};