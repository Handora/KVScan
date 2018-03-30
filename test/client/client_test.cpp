#include "client/client.h"
#include "server/server.h"
#include "gtest/gtest.h"
#include <iostream>

namespace kvscan {
  TEST(CLIENT_TEST, SimpleTest) {
    system("python3 ../script/generate.py");
    Server server("51202"); 
    server.server_->bind("Open", [&server](id_t id) { server.Open(id); });
    server.server_->bind("Close", [&server](id_t id) { server.Close(id); });
    server.server_->bind("HasNext", [&server](id_t id) { return server.HasNext(id); });
    server.server_->bind("Next", [&server](id_t id) -> Page { return server.Next(id); }); 
    server.server_->async_run(4); 
    ScanClient client("127.0.0.1", "51202");
    std::string data(58, 'a');
    std::string prefix_key(56, 'a');
    client.Open();
    int i = 0;
    int inc = -1;
    while (client.HasNext()) {
      auto p = client.Next();
      EXPECT_EQ(p.second, data);
      if (i % 4 == 0) {
	inc++;
      }
      char k1 = 'a' + inc;
      char k2 = 'a' + i % 4; 
      std::string key = prefix_key + k1 + k2;
      EXPECT_EQ(key, p.first);
      i++;
    }
    client.Close();
  }

  TEST(CLIENT_TEST, SimpleTest2) {
    system("python3 ../script/generate2.py");
    Server server("51202", "/tmp/kvscan/scan2.db"); 
    server.server_->bind("Open", [&server](id_t id) { server.Open(id); });
    server.server_->bind("Close", [&server](id_t id) { server.Close(id); });
    server.server_->bind("HasNext", [&server](id_t id) { return server.HasNext(id); });
    server.server_->bind("Next", [&server](id_t id) -> Page { return server.Next(id); }); 
    server.server_->async_run(4); 
    ScanClient client("127.0.0.1", "51202");
    std::string data(58, 'a');
    std::string prefix_key(56, 'a');

    client.Open();
    int i = 0;
    int inc = -1;

    while (client.HasNext()) {      
      auto p = client.Next(); 
      EXPECT_EQ(p.second, data);
      if (i % 4 == 0) {
	inc++;
      }
      char k1 = 'a' + inc;
      char k2 = 'a' + i % 4; 
      std::string key = prefix_key + k1 + k2;
      EXPECT_EQ(key, p.first);
      i++;
    }
    client.Close();
  }
}
