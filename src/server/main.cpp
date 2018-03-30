#include "server/server.h"

int main()
{
  // initial the server
  kvscan::Server server("51202");

  // blocking handle the rpc
  server.SyncRun();

  // if you want to run with 4 thread
  server.AsyncRun(4);
  
  return 0;
}

