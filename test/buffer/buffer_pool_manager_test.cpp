/** 
 * buffer_pool_manager_test.cpp
 * @author: Handora
 * @email: qcdsr970209@gmail.com
 */

#include <cstdio>
#include <string>

#include "buffer/buffer_pool_manager.h"
#include "gtest/gtest.h"

namespace kvscan {

  TEST(BufferPoolManagerTest, SimpleTest) {
    std::string db = "/tmp/kvscan/test.db";
    DiskManager* disk_manager = new DiskManager(db);
    BufferPoolManager* buffer_pool_manager = new BufferPoolManager(10, disk_manager);

    page_id_t page_id;
    auto page0 = buffer_pool_manager->NewPage(page_id);
    EXPECT_NE(nullptr, page0);
    EXPECT_EQ(0, page_id);

    ASSERT_NE(nullptr, page0);
    strcpy(page0->GetData(), "Hello, world");
    EXPECT_EQ(0, strcmp(page0->GetData(), "Hello, world"));
    for (int i=1; i<11; i++) {
      EXPECT_NE(nullptr, buffer_pool_manager->NewPage(page_id));
    }

    page0 = buffer_pool_manager->FetchPage(0);
    EXPECT_EQ(0, strcmp(page0->GetData(), "Hello, world"));
    remove(db.c_str());
  }  
}
