#include <asio.hpp>
#include <ctime>
#include <inttypes.h>
#include <iostream>
#include <string>

using asio::ip::udp;

int main() {
  asio::io_context io_context;
  asio::error_code error;
  std::array<uint8_t, 1> send;
  std::array<uint8_t, 1> recv;
  while (true) {
    udp::socket socket(io_context, udp::endpoint(udp::v4(), 3000));
    udp::endpoint remote_endpoint;
    socket.receive_from(asio::buffer(recv), remote_endpoint);
    send[0] = recv[0] + 1;
    socket.send_to(asio::buffer(send), remote_endpoint, 0, error);
    std::cout << "Received: " << +recv[0] << ", Sent: " << +send[0]
              << std::endl;
  }

  return 0;
}
