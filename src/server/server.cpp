/** server.cpp
 *  @author: handora
 *  @mail: qcdsr970209@gmail.com
 */

#include "server/server.h"
#include "page/page.h"
#include <iostream>

namespace kvscan {

  Page fake_page;
  
  Server::Server(const std::string port, const std::string file)
    : port_(port),
      root_(0)  {
    server_ = std::make_shared<rpc::server>(stoi(port));
    disk_manager_ = new DiskManager(file);
    buffer_pool_manager_ = std::make_shared<BufferPoolManager>(1000, disk_manager_);
    fake_page.SetPageId(INVALID_PAGE_ID);
  }

  Server::~Server() {
    // remove("/tmp/kvscan/scan.db");
  }

  void Server::Open(id_t id) {
    id_page_map_.insert(std::make_pair(id, std::make_pair(INITIAL_PAGE_ID, NOFOUND_PAGE_ID)));
  }

  void Server::Close(id_t id) {
    id_page_map_.erase(id);
  }

  bool Server::HasNext(id_t id) {
    auto itr = id_page_map_.find(id);
    if (itr == id_page_map_.end()) {
      return false;
    } 
    
    page_id_t current_page_id = itr->second.first;
    page_id_t next_page_id = itr->second.second;
    
    if (current_page_id == INITIAL_PAGE_ID) {
      return true;
    }
    
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

  Page Server::Next(id_t id) {
    auto itr = id_page_map_.find(id);
    if (itr == id_page_map_.end()) {
      return fake_page;
    }
    
    page_id_t current_page_id = itr->second.first;
    
    if (current_page_id == INITIAL_PAGE_ID) {
      id_page_map_[id] = {root_, NOFOUND_PAGE_ID};
      return *(buffer_pool_manager_->FetchPage(root_));
    } 
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
} // namespace kvscan
