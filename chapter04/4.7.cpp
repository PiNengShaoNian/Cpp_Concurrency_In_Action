// 使用std::async来将参数传递给函数
#include <future>
#include <string>

struct X {
  void foo(int, std::string const &);
  std::string bar(std::string const &);
};

X x;
// 调用p->foo(42,"hello"),其中p是&x
auto f1 = std::async(&X::foo, &x, 42, "hello");
// 调用tmpx.bar("goodbye"),其中tmpx是x的副本
auto f2 = std::async(&X::foo, x, 42, "goodbye");

struct Y {
  double operator()(double);
};
Y y;
// 调用tmpy(3.131),其中tm
auto f3 = std::async(Y(), 3.141);
// 调用y(2.718),
auto f4 = std::async(std::ref(y), 2.718);

X baz(X &);
// 调用baz(x)
auto f6 = std::async(baz, std::ref(x));
// 这个类将拷贝构造函数和赋值构造函数都取消了，只留下移动构造函数
class move_only {
 public:
  move_only();
  move_only(move_only &&);
  move_only(move_only const &) = delete;
  move_only &operator=(move_only &&);
  move_only &operator=(move_only const &) = delete;
  void operator()();
};
// 调用tmp(),其中tmp是从std::move(move_only())构造的
auto f5 = std::async(move_only());