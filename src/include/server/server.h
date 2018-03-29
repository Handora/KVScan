/** server.h
 *  @author: handora
 *  @mail: qcdsr970209@gmail.com
 */

#pragma once
#include <iostream>
#include <map>
#include <memory>

#include "rpc/server.h"
#include "page/page.h"
#include "buffer/buffer_pool_manager.h"
#include "disk/disk_manager.h"

namespace kvscan {

  class Server {
    typedef int id_t;
  public:
    Server(const std::string port="51202");
    ~Server();
    void Open(id_t id);
    void Close(id_t id);
    bool HasNext(id_t id);
    Page Next(id_t id);
    std::shared_ptr<rpc::server> server_;
  private:
    std::string port_; 
    std::map<id_t, std::pair<page_id_t, page_id_t>> id_page_map_;
    DiskManager* disk_manager_;
    std::shared_ptr<BufferPoolManager> buffer_pool_manager_;
    page_id_t root_;
  };

} // namespace kvscan
