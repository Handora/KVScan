/** client.cpp
 *  @author: handora
 *  @mail: qcdsr970209@gmail.com
 */
#include "client/client.h"

namespace kvscan {
  ScanClient::ScanClient(std::string address, std::string port): id_(++id_seed_) {
    address_ = std::move(address);
    port_ = std::move(port); 
  }

  void ScanClient::Open() {
    client_ = std::make_shared<rpc::client>(address_, port_);
    client_->call("Open", id_);
  }

  void ScanClient::Close() {
    client_->call("Close", id_);
    client_ = nullptr;
  }

  std::pair<std::string, std::string> ScanClient::Next() {
    if (current_page_ == nullptr || now_pointer_ >= 512 ) {
      if (!this->HasNext()) {
	return {"", ""};
      }
      current_page_ = std::make_shared<Page>(client_->call("Next", id_).as<Page>());
      now_pointer_ = 16;
    }

    // TODO error handler
    int n = ReadInt(current_page_->GetData()+now_pointer_);
    assert(n>=0);
    if (n == 0) {
      now_pointer_ = 512;
      return this->Next();
    }
    std::string key = ReadString(current_page_->GetData()+now_pointer_+4, n);
    now_pointer_ += n + 4;
    n = ReadInt(current_page_->GetData()+now_pointer_);
    assert(n>=0);
    if (n == 0) {
      assert(false);
    }
    std::string value = ReadString(current_page_->GetData()+now_pointer_+4, n);
    now_pointer_ += n + 4;
    return {key, value};
  }

  void ScanClient::Rewind() {
    Close();
    Open();
  }

  bool ScanClient::HasNext() {
    bool has_next = client_->call("HasNext", id_).as<bool>();
    return has_next;
  }  
}
