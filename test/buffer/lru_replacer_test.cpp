/**
 * lru_replacer_test.cpp
 */

#include "buffer/lru_replacer.h"
#include "page/page.h"
#include "gtest/gtest.h"
#include <vector>

namespace kvscan {

  TEST(LRUReplacerTest, SimpleTest) {
    LRUReplacer replacer;

    std::vector<Page*> vec;
    for (int i=0; i < 6; ++i) {
      Page *p = new Page();
      p->SetPageId(i+1);
      vec.push_back(p);
    }

    
    for (int i=0; i < 10; ++i) {
      replacer.Insert(vec[i % 6]);
    }
    EXPECT_EQ(6, replacer.Size()); 

    Page *value = nullptr;
    replacer.Victim(value);
        
    EXPECT_EQ(5, value->GetPageId());
    replacer.Victim(value);
    EXPECT_EQ(6, value->GetPageId());
    replacer.Victim(value);
    EXPECT_EQ(1, value->GetPageId());

    EXPECT_EQ(false, replacer.Erase(vec[4]));
    EXPECT_EQ(true, replacer.Erase(vec[2]));
    EXPECT_EQ(2, replacer.Size());

    replacer.Victim(value);
    EXPECT_EQ(2, value->GetPageId());
    replacer.Victim(value);
    EXPECT_EQ(4, value->GetPageId());
  }
}
