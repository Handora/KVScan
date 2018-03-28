/**
 * page.h
 * Wrapper around actual data in memory and keep the meta data
 */

#pragma once

#include <cstring>
#include "config/util.h"

namespace kvscan {
  class Page {
  public:
    Page() { memset(data_, 0, PAGE_SIZE); max_size_ =  }
    ~page() {};
    page_id_t GetPageId() const { return page_id_; };
    void SetPageId(page_id_t page_id) { page_id_ = page_id; };
    page_id_t GetParentPageId() const { return parent_page_id_; };
    void SetPageId(page_id_t page_id) { parent_page_id_ = page_id; };
    page_id_t GetLeftChildPageId() const { return left_child_page_id_; };
    void SetLeftChildPageId(page_id_t page_id) { left_child_page_id_ = page_id; };
    page_id_t GetRightChildPageId() const { return right_child_page_id_; };
    void SetRightChildPageId(page_id_t page_id) { right_child_page_id_ = page_id; };
    

    int GetSize() const { return size_; }
    void SetSize(int size) { size_ = size; }
    void IncreaseSize(int amount) { size_ += amount; };

    int GetMaxSize() const { return max_size_; };
    void SetMaxSize(int max_size) { max_size_ = max_size; };

    char *GetData() { return data_; }
    
    
  private:
    char data_[PAGE_SIZE];
    page_id_t page_id_ = INVALID_PAGE_ID;
    page_id_t parent_page_id_ = INVALID_PAGE_ID;
    page_id_t left_child_page_id_ = INVALID_PAGE_ID;
    page_id_t right_child_page_id_ = INVALID_PAGE_ID;
    // Is this page the first 100 pages of the all
    bool is_first_100_;
    // how many object can be saved in page
    int max_size_;
    // how many object now saved in page
    int size_; 
  };
} // namespace kvscan
