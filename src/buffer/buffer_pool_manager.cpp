/** 
 * buffer_pool_manager.cpp
 * @author: Handora
 * @email: qcdsr970209@gmail.com
 */
#include <iostream>
#include "buffer/buffer_pool_manager.h"
#include "page/page.h"

namespace kvscan {

  BufferPoolManager::BufferPoolManager(size_t pool_size,
				       DiskManager *disk_manager)
    : pool_size_(pool_size), disk_manager_(disk_manager) {
    // a consecutive memory space for buffer pool
    pages_ = new Page[pool_size_];
    replacer_ = new LRUReplacer();
    free_list_ = new std::list<Page *>;
    page_table_ = new std::map<page_id_t, Page*>();
    
    // put all the pages into free list
    for (size_t i = 0; i < pool_size_; ++i) {
      free_list_->push_back(&pages_[i]);
    }
  }

  BufferPoolManager::~BufferPoolManager() {
    delete[] pages_; 
    delete replacer_;
    delete free_list_;
    delete page_table_;
  }

  Page *BufferPoolManager::FetchPage(page_id_t page_id) {
    assert(page_id != INVALID_PAGE_ID); 
    std::lock_guard<std::mutex> latch(latch_);
    Page* page = nullptr;
    auto it = page_table_->find(page_id);

    if (it != page_table_->end()) {
      replacer_->Insert(it->second);
      page = it->second; 
      return page;
    }
    
    if (!free_list_->empty()) {
      // find from free list
      page = free_list_->front();
      free_list_->pop_front();
    } else {
      // find from lru replacer 
      replacer_->Victim(page);
      
      // TODO, use pin to judge whether to write back
      disk_manager_->WritePage(page->GetPageId(), page->GetData());
      page_table_->erase(page->GetPageId()); 
    } 
    page->SetPageId(page_id);
    disk_manager_->ReadPage(page_id, page->GetData());
    page_table_->insert(std::make_pair(page_id, page));
    replacer_->Insert(page);
    page->InitMeta();
    return page;
  }
  
  Page *BufferPoolManager::NewPage(page_id_t &page_id) {
    std::lock_guard<std::mutex> latch(latch_);
    
    page_id = disk_manager_->AllocatePage();
    Page *page = nullptr;
  
    if (!free_list_->empty()) {
      // find from free list
      page = free_list_->front();
      free_list_->pop_front();
    } else {
      // find from lru replacer 
      replacer_->Victim(page); 
      
      // TODO, use pin to judge whether to write back
      disk_manager_->WritePage(page->GetPageId(), page->GetData());
      page_table_->erase(page->GetPageId()); 
    } 
    page->SetPageId(page_id);
    page_table_->insert(std::make_pair(page_id, page));
    memset(page->GetData(), 0, PAGE_SIZE);
    replacer_->Insert(page);
    return page;
  }

} // namespace kvscan
