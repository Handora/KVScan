/** 
 * client.cpp
 * @author: handora
 * @mail: qcdsr970209@gmail.com
 * client api 
 */

#include "client/client.h"
#include "rpc/client.h"
#include <cstdio>
#include <cstring>

namespace kvscan {
  ScanClient::ScanClient(std::string address, std::string port){
    // seed for identifier
    // TODO:
    // seed for multiple computer
    id_ = ++ScanClient::id_seed_;
    address_ = std::move(address);
    port_ = std::move(port);
    client_ = std::make_shared<rpc::client>(address_, stoi(port_));
  }

  /**
   * Open
   * Rpc to record the session of id
   */
  void ScanClient::Open() {
    client_->call("Open", id_);
  }

  /**
   * Close
   * Rpc to end the session of id
   */
  void ScanClient::Close() {
    client_->call("Close", id_);
    client_ = nullptr;
  }

  /**
   * Next
   * Get the next k/v
   */
  std::pair<std::string, std::string> ScanClient::Next() {
    // if the current page has been read or is null
    if (current_page_ == nullptr || now_pointer_ >= 512 ) {
      // if hasn't next one just return
      if (!this->HasNext()) {
	return {"", ""};
      }

      // if has, just get the next page
      current_page_ = std::make_shared<Page>(client_->call("Next", id_).as<Page>()); 
      memcpy(current_page_->GetData(), current_page_->str_.c_str(), 512);    
      now_pointer_ = 16;
    }

    // TODO error handler 
    int n = ReadInt(current_page_->GetData()+now_pointer_); 
    assert(n>=0);
    // if 0 means padding
    // TODO:
    // 0 means padding between strings
    // Ican do better
    if (n == 0) {
      now_pointer_ = 512;
      return this->Next();
    }

    // get key
    std::string key = ReadString(current_page_->GetData()+now_pointer_+4, n);
    now_pointer_ += n + 4;
    n = ReadInt(current_page_->GetData()+now_pointer_);
    assert(n>=0);
    if (n == 0) {
      assert(false);
    }

    // get value
    std::string value = ReadString(current_page_->GetData()+now_pointer_+4, n);
    now_pointer_ += n + 4;
    return {key, value};
  }

  /**
   * Rewind
   * rewind the operation
   */ 
  void ScanClient::Rewind() {
    Close();
    Open();
  }

  /**
   * HasNext
   * Whether the next one exists
   */
  bool ScanClient::HasNext() {
    // if current page is in use, read it
    // or we just rpc for the next one
    if (current_page_ != nullptr && now_pointer_ < 512) {
      int n = ReadInt(current_page_->GetData()+now_pointer_);
      if (n != 0)
	return true;
    } 
    return client_->call("HasNext", id_).as<bool>(); 
  }  
}
