// client
#include "client/client.h"
#include <iostream>
#include <functional>
#include <string>

int main()
{
  // initial the client
  kvscan::ScanClient client("127.0.0.1", "51202");

  // Opne the rpc
  client.Open();

  // Java style iterator
  while (client.HasNext()) {
    // get the next
    auto res = client.Next();
    // output rge answer
    std::cout << res.first << " " << res.second << std::endl;
  }
  return 0;
}
