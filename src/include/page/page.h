/**
 * page.h
 * Wrapper around actual data in memory and keep the meta data
 */

#pragma once

#include <string>
#include <cstring>
#include "config/util.h"
#include <memory>
#include "rpc/server.h"

namespace kvscan {
  class BufferPoolManager;
  class Page {
    /*
     * TODO
     *   1. we can just put meta data in memory while catch the real data from buffer
     *   2. we can just write the found next id, and save it to prevent from reading again
     */
    
  public:
    Page() { memset(data_, 0, PAGE_SIZE); }
    ~Page() {};
    page_id_t GetPageId() const { return page_id_; };
    void SetPageId(page_id_t page_id) { page_id_ = page_id; };
    page_id_t GetParentPageId() const { return parent_page_id_; };
    void SetParentPageId(page_id_t page_id) { parent_page_id_ = page_id; };
    page_id_t GetLeftChildPageId() const { return left_child_page_id_; };
    void SetLeftChildPageId(page_id_t page_id) { left_child_page_id_ = page_id; };
    page_id_t GetRightChildPageId() const { return right_child_page_id_; };
    void SetRightChildPageId(page_id_t page_id) { right_child_page_id_ = page_id; };

    char *GetData() { return data_; }

    static page_id_t NextPageId(page_id_t page_id, std::shared_ptr<BufferPoolManager> buffer_pool_manager);

    static page_id_t PrevPageId(const page_id_t& page_id, std::shared_ptr<BufferPoolManager> buffer_pool_manager);

    void InitMeta();
    MSGPACK_DEFINE_ARRAY(str_, data_, page_id_, parent_page_id_, left_child_page_id_, right_child_page_id_);
    std::string str_;
  private: 
    char data_[PAGE_SIZE];
    page_id_t page_id_ = INVALID_PAGE_ID;
    page_id_t parent_page_id_ = INVALID_PAGE_ID;
    page_id_t left_child_page_id_ = INVALID_PAGE_ID;
    page_id_t right_child_page_id_ = INVALID_PAGE_ID;
    // Is this page the first 100 pages of the all
    //     bool is_first_100_;
    // how many object can be saved in page 
    // how many object now saved in page
    
  };
} // namespace kvscan
