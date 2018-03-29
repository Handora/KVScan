#include "page/page.h"
#include "buffer/buffer_pool_manager.h"

namespace kvscan {

  /* page format (keys are stored in increasing order):
   *  --------------------------------------------------------------------------
   * | Page_id | Parent_page_id | Left_child_page_id | Right_child_page_id | Real Data |
   *  --------------------------------------------------------------------------
   */


  // TODO
  //   1. we can just put meta data in memory while catch the real data from buffer
  //   2. we can just write the found next id, and save it to prevent from reading again
  
  page_id_t NextPageId(const page_id_t& page_id, std::shared_ptr<BufferPoolManager> buffer_pool_manager) {
    if (page_id == INVALID_PAGE_ID)
      return INVALID_PAGE_ID;
    Page* page = buffer_pool_manager->FetchPage(page_id);
    assert(page != nullptr);
    page_id_t right_page_id = page->GetRightChildPageId();

    if (right_page_id == INVALID_PAGE_ID) {
      return NextPageId(page->GetParentPageId(), buffer_pool_manager);
    }

    page = buffer_pool_manager->FetchPage(page_id);
    
    while (page->GetLeftChildPageId() != INVALID_PAGE_ID) {
      page = buffer_pool_manager->FetchPage(page_id);
    }

    return page->GetPageId();
  }

  void Page::InitMeta() {
    page_id_ = ReadInt(data_);
    parent_page_id_ = ReadInt(data_+4);
    left_child_page_id_ = ReadInt(data_+8);
    right_child_page_id_ = ReadInt(data_+12);
  }
  
} // namespace kvscan
