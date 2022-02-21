#ifndef _APP_H_
#define _APP_H_

#include <queue>
#include <string>

#include "message.h"

struct App {
  int ip_addr;
  int port;
  std::queue<Message> messages;
  App(int ip_addr, int port) : ip_addr(ip_addr), port(port) {}
};

#endif // _APP_H_
