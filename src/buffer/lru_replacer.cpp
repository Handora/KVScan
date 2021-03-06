/**
 * lru_replacer.h
 * author: handora
 * email: qcdsr970209@gmail.com
 * LRU bufferpool algorithm
 */

#include "buffer/lru_replacer.h"

namespace kvscan {
  LRUReplacer::LRUReplacer() {}

  LRUReplacer::~LRUReplacer() {}

  /**
   * Insert value into LRU
   */
  void LRUReplacer::Insert(Page* value) {
    // RAII like latch
    std::lock_guard<std::mutex> latch(lru_replacer_latch_);

    // if there is, put it to the first else
    // insert it to the first
    auto itr = lru_key_itr_map_.find(value);
    if (itr == lru_key_itr_map_.end()) {
      lru_list_.push_front(value);
      lru_key_itr_map_.insert({value, lru_list_.begin()});
    } else {
      lru_list_.erase(itr->second);
      lru_list_.push_front(itr->first);
      itr->second = lru_list_.begin();
    }
  }

  /**
   * victim the oldest k/v based on lru
   */
  bool LRUReplacer::Victim(Page* &value) {
    std::lock_guard<std::mutex> latch(lru_replacer_latch_);
    if (! lru_list_.empty()) {
      value = lru_list_.back();
      lru_list_.pop_back();
      lru_key_itr_map_.erase(value);
      return true;
    }
    return false;
  }

  /**
   * erase the choosen value pair
   */
  bool LRUReplacer::Erase(Page* value) {
    std::lock_guard<std::mutex> latch(lru_replacer_latch_);
    
    auto result = lru_key_itr_map_.find(value);
    if (result != lru_key_itr_map_.end()) {
      lru_list_.erase(result->second);
      lru_key_itr_map_.erase(value); 
      return true;
    }
    return false;
  }

  /**
   * return size
   */
  size_t LRUReplacer::Size() {
    return static_cast<size_t>(lru_list_.size());
  }
} // namespace kvscan

