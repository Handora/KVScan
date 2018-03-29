#pragma once
#include <iostream>
#include "rpc/client.h"
#include "page/page.h"
#include "config/util.h"
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
    int now_pointer_ = 16;
    static id_t id_seed_;
    std::shared_ptr<Page> current_page_ = nullptr;
  };
} // namespace kvscan
