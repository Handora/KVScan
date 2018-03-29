#include "server/server.h"
#include "gtest/gtest.h"
#include "page/page.h"
#include "server/server.h"
#include "rpc/client.h"
#include <functional>

namespace kvscan {
  TEST(Server_Test, SimpleTest) {
    Server server("51202"); 
    server.server_->bind("Open", [&server](id_t id) { server.Open(id); });
    server.server_->bind("Close", [&server](id_t id) { server.Close(id); });
    server.server_->bind("HasNext", [&server](id_t id) { return server.HasNext(id); });
    server.server_->bind("Next", [&server](id_t id) -> Page { return server.Next(id); }); 
    
    server.server_->async_run(4);

    rpc::client client("127.0.0.1", 51202);
    client.call("Open", 0);
    Page p = client.call("Next").as<Page>();
    EXPECT_EQ(p.GetPageId(), 0);
  }
} // namespace kvscan
