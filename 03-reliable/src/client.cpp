#include <asio.hpp>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream> 
#include "../include/sender.hpp"

#define NUM 10
using namespace std;

using asio::ip::tcp;

int main() {
  asio::io_context io_context;
  Sender sender(io_context, "127.0.0.1", "3000");

  // An explanation of the API
  // data_ready(): returns true if a message is available, otherwise false
  // get_msg(): gets an available message
  // request_msg(id): requests a message at id [0, NUM_MSGS)

  // You will need to make significant modifications to the logic below
  // As a starting point, run the server as follows:
  // ./server --no-delay --no-packet-drops which makes the below code work

  // As an example, you could start by requesting the first 10 messages
  
  cout << "hello" << endl;
  ofstream outfile("../data/gilgamesh_tcp.txt");
  auto request_start = std::chrono::high_resolution_clock::now();
  int32_t count = 0;
  string data_str[NUM];
  
  bool is_received[NUM] = {};
  for (int i = 0; i < NUM; i++)
    sender.request_msg(i);

  auto start = std::chrono::high_resolution_clock::now();

  while (true) {
    if (sender.data_ready()) {
      auto msg = sender.get_msg();
      if(msg.msg_id < count)
        continue;
      data_str[msg.msg_id % NUM] = string(msg.data.data(), CHUNK_SIZE);
      is_received[msg.msg_id % NUM] = true;

      while(is_received[count % NUM]){
        start = std::chrono::high_resolution_clock::now();
        outfile << data_str[count % NUM];
        is_received[count % NUM] = false;
        count += 1;
        if(count + NUM < NUM_MSGS)
          sender.request_msg(count + NUM - 1);
        if(count == NUM_MSGS - 1)
          goto END;
      }
    }
    
    auto rtt = std::chrono::high_resolution_clock::now() - start;
    if(std::chrono::duration_cast<std::chrono::milliseconds>(rtt).count() > 550){
      sender.request_msg(count);
      
      start = std::chrono::high_resolution_clock::now();
    }
  }
  END:
  outfile.close();
  auto request_time = std::chrono::high_resolution_clock::now() - request_start;
  std::cout << "Time: " << std::chrono::duration_cast<std::chrono::seconds>(request_time).count() << std::endl;
  return 0;
}