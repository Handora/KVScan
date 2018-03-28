#include <list>
#include <mutex>
#include <cassert>

#include "page/page.h"
#include "map"
#include "disk/disk_manager.h"
#include "buffer/lru_replacer.h"

namespace kvscan {
  class BufferPoolManager {
  public:
    BufferPoolManager(size_t pool_size, DiskManager *disk_manager);
    
    ~BufferPoolManager();

    Page *FetchPage(page_id_t page_id);

    bool UnpinPage(page_id_t page_id, bool is_dirty);
    
    bool FlushPage(page_id_t page_id);

    Page *NewPage(page_id_t &page_id);
  
    bool DeletePage(page_id_t page_id);

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

namespace kvscan {

/*
 * BufferPoolManager Constructor
 * When log_manager is nullptr, logging is disabled (for test purpose)
 * WARNING: Do Not Edit This Function
 */
  BufferPoolManager::BufferPoolManager(size_t pool_size,
				       DiskManager *disk_manager)
    : pool_size_(pool_size), disk_manager_(disk_manager) {
    // a consecutive memory space for buffer pool
    pages_ = new Page[pool_size_];
    replacer_ = new LRUReplacer();
    free_list_ = new std::list<Page *>;

    // put all the pages into free list
    for (size_t i = 0; i < pool_size_; ++i) {
      free_list_->push_back(&pages_[i]);
    }
  }

/*
 * BufferPoolManager Deconstructor
 * WARNING: Do Not Edit This Function
 */
  BufferPoolManager::~BufferPoolManager() {
    delete[] pages_; 
    delete replacer_;
    delete free_list_;
  }

/**
 * 1. search hash table.
 *  1.1 if exist, pin the page and return immediately
 *  1.2 if no exist, find a replacement entry from either free list or lru
 *      replacer. (NOTE: always find from free list first)
 * 2. If the entry chosen for replacement is dirty, write it back to disk.
 * 3. Delete the entry for the old page from the hash table and insert an
 * entry for the new page.
 * 4. Update page metadata, read page content from disk file and return page
 * pointer
 */
  Page *BufferPoolManager::FetchPage(page_id_t page_id) {
    assert(page_id != INVALID_PAGE_ID);
    std::lock_guard<std::mutex> latch(latch_);
    Page* page = nullptr;
    bool ok = page_table_.find(page_id, page);

    // if exist, pin the page and return immediately
    if (ok) {
      if (page->pin_count_ == 0)
	replacer_->Erase(page);
      page->pin_count_ += 1;
      return page;
    }
  
    if (!free_list_->empty()) {
      // find from free list
      page = free_list_->front();
      free_list_->pop_front();
    } else {
      // find from lru replacer 
      bool ok = replacer_->Victim(page);
      if (!ok) {
	// all page are pinned
	return nullptr;
      }
      page_table_->Remove(page->page_id_);
      if (page->is_dirty_) {
	disk_manager_->WritePage(page->page_id_, page->data_);
      } 
    } 
    page->is_dirty_ = false; 
    page->pin_count_ = 1;
    page->page_id_ = page_id;
    disk_manager_->ReadPage(page_id, page->data_);
    page_table_->Insert(page_id, page); 
    return page;
  }

/*
 * Implementation of unpin page
 * if pin_count>0, decrement it and if it becomes zero, put it back to
 * replacer if pin_count<=0 before this call, return false. is_dirty: set the
 * dirty flag of this page
 */
  bool BufferPoolManager::UnpinPage(page_id_t page_id, bool is_dirty) {
    assert(page_id != INVALID_PAGE_ID);
    std::lock_guard<std::mutex> latch(latch_);
    Page *page = nullptr;
    bool ok = page_table_->Find(page_id, page);
    if (ok && page->pin_count_ > 0) {
      if (--page->pin_count_ == 0) {
	replacer_->Insert(page);
      }
      if (is_dirty) {
	page->is_dirty_ = is_dirty;
      }
      return true;
    }
    return false;
  }

/*
 * Used to flush a particular page of the buffer pool to disk. Should call the
 * write_page method of the disk manager
 * if page is not found in page table, return false
 * NOTE: make sure page_id != INVALID_PAGE_ID
 */
  bool BufferPoolManager::FlushPage(page_id_t page_id) {
    assert(page_id != INVALID_PAGE_ID);
    std::lock_guard<std::mutex> latch(latch_);
    Page *page = nullptr;
    bool ok = page_table_->Find(page_id, page);

    if (ok) {
      disk_manager_->WritePage(page_id, page->data_);
      return true;
    }
    return false;
  }

/**
 * User should call this method for deleting a page. This routine will call
 * disk manager to deallocate the page. First, if page is found within page
 * table, buffer pool manager should be reponsible for removing this entry out
 * of page table, reseting page metadata and adding back to free list. Second,
 * call disk manager's DeallocatePage() method to delete from disk file. If
 * the page is found within page table, but pin_count != 0, return false
 */
  bool BufferPoolManager::DeletePage(page_id_t page_id) {
    assert(page_id != INVALID_PAGE_ID);
    std::lock_guard<std::mutex> latch(latch_);
    Page *page = nullptr;
    bool ok = page_table_->Find(page_id, page);

    if (ok && page->pin_count_ != 0) {
      page->is_dirty_ = false;
      page->pin_count_ = 0;
      page->page_id_ = INVALID_PAGE_ID; 
      page_table_->Remove(page_id);
      free_list_->push_back(page);
      disk_manager_->DeallocatePage(page_id);
      return true;
    } else if (!ok) {
      disk_manager_->DeallocatePage(page_id);
      return true;
    }

    return false;
  }

/**
 * User should call this method if needs to create a new page. This routine
 * will call disk manager to allocate a page.
 * Buffer pool manager should be responsible to choose a victim page either
 * from free list or lru replacer(NOTE: always choose from free list first),
 * update new page's metadata, zero out memory and add corresponding entry
 * into page table. return nullptr if all the pages in pool are pinned
 */
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
      bool ok = replacer_->Victim(page);
      if (!ok) {
	// all page are pinned
	return nullptr;
      }
      page_table_->Remove(page->page_id_);
      if (page->is_dirty_) {
	disk_manager_->WritePage(page->page_id_, page->data_);
      } 
    } 
    page->is_dirty_ = true; 
    page->pin_count_ = 1;
    page->page_id_ = page_id; 
    page_table_->Insert(page_id, page);
    memset(page->data_, 0, PAGE_SIZE);
    return page;
  }
} // namespace kvscan