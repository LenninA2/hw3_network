#include <asio.hpp>
#include <iostream>

using asio::ip::udp;

int main(int argc, char *argv[]) {
  asio::io_context io_context;
  if (argc != 2) {
    std::cerr << "Usage: client <host>\n";
    return 1;
  }
  auto host = argv[1];
  udp::resolver resolver(io_context);
  udp::endpoint receiver_endpoint =
      *resolver.resolve(udp::v4(), host, "3000").begin();

  std::array<uint8_t, 1> send;
  std::array<uint8_t, 1> recv;

  for (int i = 0; i < 8; ++i) {
    udp::socket socket(io_context);
    socket.open(udp::v4());
    send[0] = i;
    socket.send_to(asio::buffer(send), receiver_endpoint);
    udp::endpoint sender_endpoint;
    size_t len = socket.receive_from(asio::buffer(recv), sender_endpoint);
    std::cout << "Sent: " << i << ", Received: " << +recv[0] << std::endl;
  }

  return 0;
}
