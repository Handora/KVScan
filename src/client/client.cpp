/** client.cpp
 *  @author: handora
 *  @mail: qcdsr970209@gmail.com
 */

#include <iostream>
#include "rpc/client.h"
#include "page/page.h"
#include <string>
#include <memory>
#include <utility>

namespace kvscan {
  class ScanClient {
    typedef int id_t;
  public:
    ScanClient(std::string address, std::string port);
    bool HasNext();
    void Open();
    void Close();
    std::pair<std::string, std::string> Next();
    void Rewind();
  private:
    std::shared_ptr<rpc::client> client_ = nullptr;
    std::string address_;
    std::string port_;
    id_t id_;
    int now_readn_ = 0;
    static id_t id_seed_;
    std::shared_ptr<Page> current_page_ = nullptr;
  };
}

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
    if (current_page_ == nullptr) {
      current_page_  = std::make_shared<Page>(client_->call("Next", id_).as<Page>());
      
    }
    return {"", ""};
  }

  void ScanClient::Rewind() {
    Close();
    Open();
  }

  bool ScanClient::HasNext() {
    while (current_page_ == nullptr || now_readn_ == current_page_->GetSize()) {
      current_page_  = std::make_shared<Page>(client_->call("Next", id_).as<Page>());
      if (current_page_->IsFinal()) {
	return false;
      }
      now_readn_ = 0;
    }

    return true;
  }  
}

int main(int argc, char *argv[])
{
  if (argc < 3) {
    std::cout << "Scanclient address port" << std::endl;
    exit(1);
  }

  std::shared_ptr<kvscan::ScanClient> client = std::make_shared<kvscan::ScanClient>(std::string(std::move(argv[1])), std::string(argv[2]));

  int i = 0;
  while(client->HasNext() && i < 100) {
    auto key_value = client->Next();
    std::cout << key_value.first << " " << key_value.second << std::endl;
  }
  return 0;
}

