#include "gtest/gtest.h"
#include "disk/disk_manager.h"
#include "config/util.h"

#include <cstdio>
#include <cstring>
#include <string>

namespace kvscan {

  TEST(DiskManagerTest, SimpleTest) {
    const std::string db= "/tmp/kvscan/test.db";
    DiskManager disk_manager(db); 
    char temp[PAGE_SIZE];
    char temp2[PAGE_SIZE];

    for (int i=0; i<10; i++) {
      memset(temp, PAGE_SIZE, i);
      page_id_t page_id = disk_manager.AllocatePage();
      disk_manager.WritePage(page_id, temp);
      disk_manager.ReadPage(page_id, temp2);
      EXPECT_EQ(memcmp(temp, temp2, PAGE_SIZE), 0);
    }

    EXPECT_EQ(disk_manager.GetFileSize(db), 10 * PAGE_SIZE);
    remove(db.c_str());
  }
}
