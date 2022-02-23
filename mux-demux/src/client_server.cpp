#include <iostream>
#include <vector>

#include "app.h"
#include "host.h"
#include "message.h"

#define CLIENT_APP_PORT 100
#define SERVER_APP_PORT 200

// Server should send the following response to the client based on the
// integer value of the request passed in. Any value larger, should return
// an error message of your choice!
std::vector<std::string> responses = {
    "Hello, world",
    "Don't panic",
    "I'm a text message",
    "I'm a binary message",
    "What time is it?",
    "I'm a very long message that is longer than the buffer size",
    "This is not python",
};

int main(int arcg, char **argv) {
  // *** Initializing apps ***
  std::vector<Host> hosts;

  // Client (ip addr 0) has one app:
  auto client = Host(0);
  auto client_app = App(client.ip_addr, CLIENT_APP_PORT);

  // Server(ip addr 1) has one app:
  auto server = Host(1);
  auto server_app = App(server.ip_addr, SERVER_APP_PORT);

  // Add apps to vector
  client.apps.push_back(client_app);
  server.apps.push_back(server_app);

  // Add hosts to vector
  hosts.push_back(client);
  hosts.push_back(server);

  // *** Initializing messages ***
  std::vector<Message> messages;

  // Write a loop to send all valid messages to the server from the client and
  // receive the response from the server. The client should be the one to
  // print the responses (server doesn't print anything!).
  for (auto i = 0; i < responses.size(); i++) {
    auto m = Message(client.ip_addr, CLIENT_APP_PORT, server.ip_addr,
                     SERVER_APP_PORT, std::to_string(i));
    server.messages.push(m);
  }

  // Forward server messages to to correct app
  while (!server.messages.empty()) {
    auto message = server.messages.front();
    server.messages.pop();

    for (auto &app : server.apps) {
      if (app.ip_addr == message.receiver_ip_addr &&
          app.port == message.receiver_port) {
        app.messages.push(message);
      }
    }
  }

  // Each app on server reads the message(s) from the queue
  for (auto &app : server.apps) {
    while (!app.messages.empty()) {
      auto message = app.messages.front();
      app.messages.pop();

      if (app.port == SERVER_APP_PORT) {
        int id = std::stoi(message.content);
        client.messages.push(Message(server.ip_addr, SERVER_APP_PORT,
                                     client.ip_addr, CLIENT_APP_PORT,
                                     responses[id]));
      }
    }
  }

  // Client forwards messages to correct app
  for (auto &app : client.apps) {
    while (!client.messages.empty()) {
      auto message = client.messages.front();
      client.messages.pop();

      if (app.ip_addr == message.receiver_ip_addr &&
          app.port == message.receiver_port) {
        app.messages.push(message);
      }
    }
  }

  // Each app on client reads the message(s) from the queue
  for (auto &app : client.apps) {
    while (!app.messages.empty()) {
      auto message = app.messages.front();
      app.messages.pop();
      std::cout << message.content << std::endl;
    }
  }

  return 0;
}
