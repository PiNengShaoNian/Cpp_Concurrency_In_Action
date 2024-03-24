// 清单4.1 使用std::condition_variable等待数据
#include <condition_variable>
#include <mutex>
#include <queue>

class data_chunk {};
std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable data_cond;

bool more_data_to_prepare();
data_chunk prepare_data();
void data_preparation_thread() {
  while (more_data_to_prepare()) {
    data_chunk const data = prepare_data();
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(data);
    data_cond.notify_one();
  }
}

void process(data_chunk data);
bool is_last_chunk(data_chunk data);

void data_processing_thread() {
  while (true) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [] { return !data_queue.empty(); });
    data_chunk data = data_queue.front();
    data_queue.pop();
    lk.unlock();
    process(data);
    if (is_last_chunk(data)) {
      break;
    }
  }
}