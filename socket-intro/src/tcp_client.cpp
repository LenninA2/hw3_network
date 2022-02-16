#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

int main(int argc, char *argv[]) {
  asio::io_context io_context;
  tcp::socket socket(io_context);
  tcp::resolver resolver(io_context);

  if (argc != 2) {
    std::cerr << "Usage: client <host>\n";
    return 1;
  }
  auto host = argv[1];
  asio::error_code error;
  std::array<uint8_t, 1> send;
  std::array<uint8_t, 1> recv;
  for (int i = 0; i < 8; ++i) {
    tcp::resolver::results_type endpoints = resolver.resolve(host, "3000");
    asio::connect(socket, endpoints);
    send[0] = i;
    socket.send(asio::buffer(send), 0, error);
    size_t len = socket.read_some(asio::buffer(recv), error);
    std::cout << "Sent: " << i << ", Received: " << +recv[0] << std::endl;
  }

  return 0;
}
