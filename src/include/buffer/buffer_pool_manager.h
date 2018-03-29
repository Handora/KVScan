#pragma once

#include <list>
#include <mutex>
#include <cassert>

#include "map"
#include "disk/disk_manager.h"
#include "buffer/lru_replacer.h"


namespace kvscan {
  class BufferPoolManager {
  public:
    BufferPoolManager(size_t pool_size, DiskManager *disk_manager);
    
    ~BufferPoolManager();

    Page *FetchPage(page_id_t page_id);

  private:
    size_t pool_size_; // number of pages in buffer pool
    Page *pages_;      // array of pages
    DiskManager *disk_manager_;
    std::map<page_id_t, Page *> *page_table_; // to keep track of pages
    LRUReplacer *replacer_;   // to find an unpinned page for replacement
    std::list<Page *> *free_list_; // to find a free page for replacement
    std::mutex latch_;             // to protect shared data structure
  };
} // namespace kvscan
