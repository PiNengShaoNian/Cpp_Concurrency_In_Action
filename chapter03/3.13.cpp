// 使用share_mutex保护数据结构
#include <map>
#include <mutex>
#include <shared_mutex>
#include <string>

class dns_entry;

class dns_cache {
  std::map<std::string, dns_entry *> entries;
  std::shared_mutex mutable entry_mutex;

 public:
  dns_entry *find_entry(std::string const &domain) const {
    std::shared_lock<std::shared_mutex> lk(entry_mutex);
    std::map<std::string, dns_entry *>::const_iterator const it =
        entries.find(domain);
    return (it == entries.end()) ? nullptr : it->second;
  }

  void update_or_add_entry(std::string const &domain,
                           dns_entry *const dns_details) {
    std::lock_guard<std::shared_mutex> lk(entry_mutex);
    entries[domain] = dns_details;
  }
};