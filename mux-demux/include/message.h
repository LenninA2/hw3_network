#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>

struct Message {
  Message(int sender_ip_addr, int sender_port, int receiver_ip_addr,
          int receiver_port, std::string content)
      : sender_ip_addr(sender_ip_addr), sender_port(sender_port),
        receiver_ip_addr(receiver_ip_addr), receiver_port(receiver_port),
        content(content) {}

  int sender_ip_addr;
  int sender_port;
  int receiver_ip_addr;
  int receiver_port;
  std::string content;
};

#endif // _MESSAGE_H_
