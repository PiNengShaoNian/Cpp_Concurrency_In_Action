// 代码清单7.7　get_hazard_pointer_for_current_thread()的简单实现
#include <atomic>
#include <memory>
#include <stdexcept>
#include <thread>

unsigned const max_hazard_pointers = 100;
struct hazard_pointer {
  std::atomic<std::thread::id> id;
  std::atomic<void *> pointer;
};

hazard_pointer hazard_pointers[max_hazard_pointers];
class hp_owner {
  hazard_pointer *hp;

 public:
  hp_owner(hp_owner const &) = delete;
  hp_owner operator=(hp_owner const &) = delete;
  hp_owner() : hp(nullptr) {
    for (unsigned i = 0; i < max_hazard_pointers; i++) {
      std::thread::id old_id;
      if (hazard_pointers[i].id.compare_exchange_strong(
              old_id, std::this_thread::get_id())) {
        hp = &hazard_pointers[i];
        break;
      }
    }
    if (!hp) {
      throw std::runtime_error("No hazard pointers available");
    }
  }

  std::atomic<void *> &get_pointer() { return hp->pointer; }

  ~hp_owner() {
    hp->pointer.store(nullptr);
    hp->id.store(std::thread::id());
  }
};

std::atomic<void *> &get_hazard_pointer_for_current_thread() {
  thread_local static hp_owner hazard;
  return hazard.get_pointer();
}

bool outstanding_hazard_pointers_for(void *p) {
  for (unsigned i = 0; i < max_hazard_pointers; i++) {
    if (hazard_pointers[i].pointer.load() == p) {
      return true;
    }
  }
  return false;
}