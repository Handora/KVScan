/**
 * lru_replacer.h
 * author: handora
 * email: qcdsr970209@gmail.com
 * LRU bufferpool algorithm
 */

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
    // insert value into the first else put it to the first
    void Insert(Page* value);
    // victim the oldest one
    bool Victim(Page* &value);
    // erase a specific page
    bool Erase(Page* value);
    
    size_t Size();

  private: 
    std::unordered_map<Page*, std::list<Page*>::iterator> lru_key_itr_map_; // map for pointer
    std::list<Page*> lru_list_; // lru list
    std::mutex lru_replacer_latch_; // latch for cc
  };
} // namespace kvscan
