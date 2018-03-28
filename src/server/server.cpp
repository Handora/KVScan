/** client.cpp
 *  @author: handora
 *  @mail: qcdsr970209@gmail.com
 */

#include <iostream>
#include "rpc/server.h"
#include "page/page.h"
#include <map>
#include <memory>

namespace kvscan {

  class Server {
    typedef int id_t;
  public:
    Server(const std::string port="51202");
    void Open(id_t id);
    void Close(id_t id);
    bool HasNext(id_t id);
    Page Next(id_t id);
  private:
    std::string port_;
    std::shared_ptr<rpc::server> server_;
  };
} // namespace kvscan

namespace kvscan {
  Server::Server(const std::string port): port_(port) {
    server_ = std::make_shared<rpc::server>(stoi(port));
  }

  void Server::Open(id_t id) {
    id = 1;
    return ;
  }
} // namespace kvscan


int main() {
  // Creating a server that listens on port 8080
  rpc::server srv(8080);

  // Binding the name "foo" to free function foo.
  // note: the signature is automatically captured 

  // Binding a lambda function to the name "add".
  srv.bind("add", [](int a, int b) {
      return a + b;
    });

  // Run the server loop.
  srv.run();

  return 0;
}
