#include <iostream>
#include <vector>

#include "app.h"
#include "host.h"
#include "message.h"

#define HOST_A_APP_1 100
#define HOST_B_APP_1 200
#define HOST_B_APP_2 300

int main(int arcg, char **argv) {
  // *** Initializing apps ***
  std::vector<Host> hosts;

  // Host a (ip addr 0) has one app:
  auto host_a = Host(0);
  auto host_a_app_1 = App(host_a.ip_addr, HOST_A_APP_1);

  // Host b (ip addr 1) has two app:
  auto host_b = Host(1);
  auto host_b_app_1 = App(host_b.ip_addr, HOST_B_APP_1);
  auto host_b_app_2 = App(host_b.ip_addr, HOST_B_APP_2);

  // Add apps to vector
  host_a.apps.push_back(host_a_app_1);
  host_b.apps.push_back(host_b_app_1);
  host_b.apps.push_back(host_b_app_2);

  // Add hosts to vector
  hosts.push_back(host_a);
  hosts.push_back(host_b);

  // *** Initializing messages ***
  std::vector<Message> messages;

  // *Send* messages
  host_b.messages.push(Message(host_a.ip_addr, HOST_A_APP_1, host_b.ip_addr,
                               HOST_B_APP_1, "Hello, world"));
  host_b.messages.push(Message(host_a.ip_addr, HOST_A_APP_1, host_b.ip_addr,
                               HOST_B_APP_2, "20"));

  // host b reads the message(s) from the queue
  while (!host_b.messages.empty()) {
    auto message = host_b.messages.front();
    host_b.messages.pop();

    // host b forwards the message to correct app
    for (auto &app : host_b.apps) {
      if (app.ip_addr == message.receiver_ip_addr &&
          app.port == message.receiver_port) {
        app.messages.push(message);
      }
    }
  }

  // Each app on host b reads the message(s) from the queue
  for (auto &app : host_b.apps) {
    while (!app.messages.empty()) {
      auto message = app.messages.front();
      app.messages.pop();

      // Port for HOST_B_APP_1 (Application Logic for App 1 on Host B)
      // Logic: Add 3 ! to the message content and print
      if (app.port == HOST_B_APP_1) {
        std::cout << message.content << "!!!" << std::endl;
      }
      // Port for HOST_B_APP_2 (Application Logic for App 1 on Host B)
      // Logic: Convert to int, add 1, and print
      else if (app.port == HOST_B_APP_2) {
        int i = atoi(message.content.c_str());
        std::cout << i + 1 << std::endl;
      }
    }
  }
  return 0;
}
