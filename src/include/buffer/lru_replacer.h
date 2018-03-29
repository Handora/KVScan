#pragma once

#include "page/page.h"

#include <cstdlib>
#include <list>
#include <iterator>
#include <mutex>
#include <unordered_map>

namespace kvscan {

  class LRUReplacer{
  public: 
    LRUReplacer();

    ~LRUReplacer();

    void Insert(Page* value);

    bool Victim(Page* value);

    bool Erase(Page* value);

    size_t Size();

  private: 
    std::unordered_map<Page*, std::list<Page*>::iterator> lru_key_itr_map_;
    std::list<Page*> lru_list_;

    std::mutex lru_replacer_latch_;
  };
} // namespace kvscan
