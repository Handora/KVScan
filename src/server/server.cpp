/** 
 * server.cpp
 * @author: handora
 * @mail: qcdsr970209@gmail.com
 * Server api
 */

#include "server/server.h"
#include "page/page.h"
#include "rpc/server.h"
#include <iostream>

namespace kvscan {
  // page as error or finished
  Page fake_page;

  
  Server::Server(const std::string port, const std::string file)
    : port_(port),
      root_(0)  {
    server_ = std::make_shared<rpc::server>(stoi(port));
    disk_manager_ = new DiskManager(file);
    buffer_pool_manager_ = std::make_shared<BufferPoolManager>(1000, disk_manager_);
    fake_page.SetPageId(INVALID_PAGE_ID);

    // bind the rpc call
    server_->bind("Open", [this](id_t id) { this->Open(id); });
    server_->bind("Close", [this](id_t id) { this->Close(id); });
    server_->bind("HasNext", [this](id_t id) { return this->HasNext(id); });
    server_->bind("Next", [this](id_t id) -> Page { return this->Next(id); });
  }

  Server::~Server() {
    // remove("/tmp/kvscan/scan.db");
  }

  /**
   * Open
   * Rpc to record the session of id
   */
  void Server::Open(id_t id) {
    id_page_map_.insert(std::make_pair(id, std::make_pair(INITIAL_PAGE_ID, NOFOUND_PAGE_ID)));
  }

  /**
   * Close
   * Rpc to end the session of id
   */
  void Server::Close(id_t id) {
    id_page_map_.erase(id);
  }

  /**
   * HasNext
   * Rpc whether the next page exists
   */
  bool Server::HasNext(id_t id) {
    // if not in map, the client doesn't open it, return error
    auto itr = id_page_map_.find(id);
    if (itr == id_page_map_.end()) {
      return false;
    } 
    
    page_id_t current_page_id = itr->second.first;
    page_id_t next_page_id = itr->second.second;

    // INITIAL page
    if (current_page_id == INITIAL_PAGE_ID) {
      return true;
    }

    // If the second one is NOFOUND, find whether it exsits.
    // I optimize it with memorying the result
    // Else, just return based on the second one
    if (next_page_id != NOFOUND_PAGE_ID) {
      if (next_page_id == INVALID_PAGE_ID)
	return false;
      return true;
    }
    
    next_page_id = Page::NextPageId(current_page_id, buffer_pool_manager_); 
    
    id_page_map_[id] = {current_page_id, next_page_id};
    if (next_page_id == INVALID_PAGE_ID)
      return false;
    return true;
  }

  /**
   * Next
   * Rpc the next page to client
   */ 
  Page Server::Next(id_t id) {
    // if not in map, the client doesn't open it, return error
    auto itr = id_page_map_.find(id);
    if (itr == id_page_map_.end()) {
      return fake_page;
    }
    
    page_id_t current_page_id = itr->second.first;

    // INITIAL page
    if (current_page_id == INITIAL_PAGE_ID) {
      id_page_map_[id] = {root_, NOFOUND_PAGE_ID};
      return *(buffer_pool_manager_->FetchPage(root_));
    }

    // If the second one is NOFOUND, find it.
    // Else, just return it 
    page_id_t next_page_id = itr->second.second; 
    if (next_page_id != NOFOUND_PAGE_ID) {
      if (next_page_id == INVALID_PAGE_ID)
	return fake_page;
      id_page_map_[id] = {next_page_id, NOFOUND_PAGE_ID}; 
      return *(buffer_pool_manager_->FetchPage(next_page_id));
    }
    
    next_page_id = Page::NextPageId(current_page_id, buffer_pool_manager_);
    if (next_page_id == INVALID_PAGE_ID)
      return fake_page;
    id_page_map_[id] = {next_page_id, NOFOUND_PAGE_ID};
    return *(buffer_pool_manager_->FetchPage(next_page_id));
  }

  /**
   * AsyncRun
   * async multithreaded no-blocking rpc handler
   */
  void Server::AsyncRun(int thread_num) {
    server_->async_run(thread_num);
  }

  /**
   * SyncRun
   * blocking rpc handler
   */
  void Server::SyncRun() {
    server_->run();
  }
} // namespace kvscan
