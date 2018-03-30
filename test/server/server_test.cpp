#include "server/server.h"
#include "gtest/gtest.h"
#include "page/page.h"
#include "server/server.h"
#include "rpc/client.h"
#include <functional>
#include <iostream>
#include <exception>
#include <cstdlib>

namespace kvscan {
  // TODO
  //   how to cope with system("python3 ../script/generate.py");
  
  TEST(Server_Test, SimpleTest) {
    // system("python3 ../script/generate.py");
    Server server("51202");
    server.AsyncRun(4);
    rpc::client client("127.0.0.1", 51202);
    client.call("Open", 0);
    bool ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 

    Page p = client.call("Next", 0).as<Page>(); 
    EXPECT_EQ(p.GetPageId(), 0); 
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 2);
    ok = client.call("HasNext", 0).as<bool>();
    ok = client.call("HasNext", 0).as<bool>();
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>(); 
    EXPECT_EQ(p.GetPageId(), 1);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 3);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 5);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 4);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 6);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, false); 
  }

  TEST(Server_Test, SimpleTest2) {
    // system("python3 ../script/generate2.py");
    Server server("51202", "/tmp/kvscan/scan2.db");
    server.AsyncRun(4);
    rpc::client client("127.0.0.1", 51202);
    client.call("Open", 0); 
    Page p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 0); 
    bool ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 2);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 1);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true);
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 8);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 7);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true);
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 3);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 5);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 4);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, true); 
    p = client.call("Next", 0).as<Page>();
    EXPECT_EQ(p.GetPageId(), 6);
    ok = client.call("HasNext", 0).as<bool>();
    EXPECT_EQ(ok, false); 
  }

} // namespace kvscan
