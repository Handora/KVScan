/**
 * borrow from cmu 15-445
 * diskmanager.cpp
 */

#pragma once

#include <atomic>
#include <fstream>
#include <future>
#include <string>
#include "config/util.h"

namespace kvscan {
  class DiskManager {
  public:
    DiskManager(const std::string &file);
    ~DiskManager();

    void WritePage(page_id_t page_id, const char *page_data);
    void ReadPage(page_id_t page_id, char *page_data);
    
    page_id_t AllocatePage();
    void DeallocatePage(page_id_t page_id);
    int GetFileSize(const std::string &name);
  private:
    
    // stream to write db file
    std::fstream db_io_;
    std::string file_name_;
    std::atomic<page_id_t> next_page_id_;
  };
} // namespace kvscan
