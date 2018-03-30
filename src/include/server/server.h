/** 
 * server.h
 * @author: handora
 * @mail: qcdsr970209@gmail.com
 * server header
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
    // server api
    Server(const std::string port, const std::string file="/tmp/kvscan/scan.db");
    ~Server();
    void Open(id_t id);
    void Close(id_t id);
    bool HasNext(id_t id);
    Page Next(id_t id);
    void AsyncRun(int thread_num);
    void SyncRun();
    
  private: 
    std::string port_;
    // map of id to current page and next page
    std::map<id_t, std::pair<page_id_t, page_id_t>> id_page_map_;
    // diskmanager for bufferpoolmanager
    DiskManager* disk_manager_; 
    std::shared_ptr<BufferPoolManager> buffer_pool_manager_;
    // the root of BST
    page_id_t root_;
    std::shared_ptr<rpc::server> server_;
  };

} // namespace kvscan
