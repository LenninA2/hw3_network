#ifndef _HOST_H_
#define _HOST_H_

#include <queue>
#include <vector>

#include "app.h"
#include "message.h"

struct Host {
  int ip_addr;
  std::vector<App> apps;

  // Messages to be forwarded to all apps on Host (demux)
  std::queue<Message> messages;

  Host(int ip_addr) : ip_addr(ip_addr) {}
};

#endif // _HOST_H_