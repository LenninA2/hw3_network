#include <asio.hpp>
#include <iostream>
#include <fstream>
#include "../include/sender.hpp"
#include <list>
#include <map>
#include <chrono>
#include <thread>


using asio::ip::tcp;
using namespace std::literals::chrono_literals;
using namespace std;


void print_map(map<int,string> myMap) {
  for (auto& t : myMap) {
    std::cout << t.first << "\n";
              // << t.second << "\n";
  }
}


void timer() 
{

  auto start = std::chrono::high_resolution_clock::now();
  std::this_thread::sleep_for(1s);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<float> duration = end - start;
  cout << duration.count() << endl;

  std::cin.get();
}


int main() {

  // Create a "context" - essentially the platform interface 
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


  // int32_t curr_msg = 0;
  // for (int i = 0; i < 10; i++) {

  //   //maybe add a timeout here? 
  // sender.request_msg(i);
  // curr_msg++;
  // }
  //setting up output file

  ofstream outFile ("../data/gilgamesh_tcp.txt");


  map<int, string> received_packets;

  //timeout

  int count = 0;
  while (true) {
    sender.request_msg(count);
    if (sender.data_ready()) {

      // Get a response Msg:
      // A Msg has a msg_id (corresponds to id in request_msg) and
      // a char array of CHUNK_SIZE (128) storing the data
      auto msg = sender.get_msg();

      cout << to_string(count) << ": message that was requested: " << to_string(msg.msg_id) << endl;

      // Eventually, you will combine these chunks to write the file
      auto data_str = string(msg.data.data(), CHUNK_SIZE);

      received_packets.insert(pair<int, string>(msg.msg_id, data_str));


      // Print the msg id and message recieved (may be out of order)
      //cout << "msg_id(" << msg.msg_id << ")::" << data_str << endl;
    }

    if (received_packets.size() == 10){
      break;
    } 
    count++;

    if(count < 852 ){
      break;
    }
  }

  //print_map(received_packets);
  
  outFile.close();

  return 0;
}
