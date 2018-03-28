#include <iostream>
#include "rpc/client.h"
#include "page/page.h"
#include <string>
#include <memory>

namespace kvscan {
  class ScanClient {
    typedef int id_t;
  public:
    ScanClient(std::string& address, std::string& port, id_t id);
    bool HasNext();
    void Open();
    void Close();
    std::string Next();
    void Rewind();
  private:
    std::shared_ptr<rpc::client> client_ = nullptr;
    std::string address_;
    std::string port_;
    id_t id_;
    int now_readn_ = 0;
    std::shared_ptr<Page> current_page_ = nullptr;
  };
}

namespace kvscan {
  ScanClient::ScanClient(std::string& address, std::string& port, id_t id): id_(id) {
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

  std::string ScanClient::Next() {
    
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
  if (argc < 4) {
    std::cout << "Scanclient address port id" << std::endl;
    exit(1);
  }

  kvscan::ScanClient client(argv[1], argv[2], argv[3]);
  return 0;
}

