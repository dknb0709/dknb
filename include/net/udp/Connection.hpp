#ifndef __DKNB_NET_TCP_COMMON__
#define __DKNB_NET_TCP_COMMON__

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>

namespace dknb {
namespace net {

struct Socket {
  int socket_;
  sockaddr_in addr_;

  Socket(const std::string &address, uint32_t port) {
    // create socket
    socket_ = ::socket(AF_INET, SOCK_STREAM, 0);

    // configure
    addr_.sin_family = AF_INET;
    addr_.sin_port = ::htons(port);

    // resolve host name
    addr_.sin_addr.s_addr = inet_addr(address.c_str());
    if (addr_.sin_addr.s_addr == 0xffffffff) {
      hostent *host = ::gethostbyname(address.c_str());
      if (host != NULL) {
        addr_.sin_addr.s_addr = *(uint32_t *)host->h_addr_list[0];
      }
    }
  }
  ~Socket() { ::close(socket_); }
};

/**
 * server/client
 * blocking/non blocking
 * tcp/udp
 * keepalive?
 */
template <uint32_t BufSize = 1024>
class Connection {
  Socket sock_;

 public:
  Connection(const std::string &address, uint32_t port) : sock_{address, port} {
    ::connect(sock_.socket_, (sockaddr *)&sock_.addr_, sizeof(sock_.addr_));
  }
  void send(const std::string &msg) {
    uint32_t nSend = 0;

    while (nSend < msg.size()) {
      uint32_t last = std::max((uint32_t)msg.size(), nSend + BufSize);
      nSend +=
          ::send(sock_.socket_, msg.substr(nSend, last).c_str(), BufSize, 0);
    }
  }
  std::string recv() {
    size_t nRecv = 0;
    char buf[BufSize];
    std::string response{};
    while ((nRecv = ::recv(sock_.socket_, buf, BufSize, 0)) > 0) {
      response += std::string{buf, buf + nRecv};
    }
    return response;
  }
};

}  // namespace net
}  // namespace dknb

#endif
