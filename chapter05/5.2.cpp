// 从不同的线程中读取和写入变量
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> data;
std::atomic<bool> data_ready;

void ready_thread() {
  while (!data_ready.load()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  std::cout << "The answer=" << data[0] << "\n";
}

void writer_thread() {
  data.push_back(42);
  data_ready = true;
}