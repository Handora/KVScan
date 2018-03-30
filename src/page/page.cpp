#include "page/page.h"
#include "buffer/buffer_pool_manager.h"
#include <iostream>
#include <cassert>

namespace kvscan {

  /* page format (keys are stored in increasing order):
   *  --------------------------------------------------------------------------
   * | Page_id | Parent_page_id | Left_child_page_id | Right_child_page_id | Real Data |
   *  --------------------------------------------------------------------------
   */


  // TODO
  //   1. we can just put meta data in memory while catch the real data from buffer
  //   2. we can just write the found next id, and save it to prevent from reading again !!ok, implemented!!

  /**
   * NextPageid
   * implement the BST with state machine
   */
  page_id_t Page::NextPageId(page_id_t page_id, std::shared_ptr<BufferPoolManager> buffer_pool_manager) {
    if (page_id == INVALID_PAGE_ID)
      return INVALID_PAGE_ID; 
    Page* page = buffer_pool_manager->FetchPage(page_id);
    assert(page != nullptr);
    page_id_t right_page_id = page->GetRightChildPageId();

    // handle the next one and go up
    if (right_page_id == INVALID_PAGE_ID) {
      if (page->GetParentPageId() == INVALID_PAGE_ID) {
	return INVALID_PAGE_ID;
      } 
      page = buffer_pool_manager->FetchPage(page->GetParentPageId());
      while (page->GetRightChildPageId() == page_id) {
	page_id = page->GetPageId();
	if (page->GetParentPageId()==INVALID_PAGE_ID)
	  return INVALID_PAGE_ID;
	page = buffer_pool_manager->FetchPage(page->GetParentPageId()); 
      } 
      return page->GetPageId();
    }

    // go left
    page = buffer_pool_manager->FetchPage(right_page_id);

    while ((page_id=page->GetLeftChildPageId()) != INVALID_PAGE_ID) {
      page = buffer_pool_manager->FetchPage(page_id);
    }

    return page->GetPageId();
  }

  /**
   * init meta data from diskmanager
   */
  void Page::InitMeta() {
    page_id_ = ReadInt(data_); 
    parent_page_id_ = ReadInt(data_+4); 
    left_child_page_id_ = ReadInt(data_+8); 
    right_child_page_id_ = ReadInt(data_+12);
    str_ = std::string(data_, 512); 
  }
  
  } // namespace kvscan
