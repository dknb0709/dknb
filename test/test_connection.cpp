#include <net/common/connection.hpp>

#define DEBUG(arg) std::cout << "[debug] " #arg " = " << (arg) << std::endl

using namespace dknb::net;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "[usage] command <address>:<port>" << std::endl;
    return 1;
  }
  std::string request{argv[1]};
  int pos = request.find(":");
  std::string address = request.substr(0, pos);
  uint32_t port = (uint32_t)std::stoi(request.substr(pos + 1, request.size()));

  DEBUG(address);
  DEBUG(port);

  // create connection
  Connection<256> conn{address, port};

  // create request message
  char msg[256]{"GET / HTTP/1.0\r\n\r\n"};

  conn.send(msg);
  std::string response = conn.recv();

  // get response
  std::cout << response << std::endl;
}
