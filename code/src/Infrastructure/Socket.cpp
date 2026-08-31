#include "Infrastructure/Socket.hpp"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <utility>

namespace {
#ifdef _WIN32
void initializeWinsock()
{
  static bool initialized = false;
  if (!initialized) {
    WSADATA data{};
    (void)WSAStartup(MAKEWORD(2, 2), &data);
    initialized = true;
  }
}
#endif

int socketClose(NativeSocket fd)
{
#ifdef _WIN32
  return ::closesocket(fd);
#else
  return ::close(fd);
#endif
}
} // namespace

Socket::Socket(NativeSocket fd, std::string address, uint16_t port) noexcept
    : m_address{std::move(address)}, m_port{port}, m_fd{fd} {}

Socket::Socket(Socket &&other) noexcept
    : m_address{std::move(other.m_address)}, m_port{other.m_port},
      m_fd{std::exchange(other.m_fd, kInvalidSocket)} {}

// The class uses the operating system's UDP socket interface to receive and reply to packets.

Socket &Socket::operator=(Socket &&other) noexcept
{
  if (this != &other) {
    close();
    m_address = std::move(other.m_address);
    m_port = other.m_port;
    m_fd = std::exchange(other.m_fd, kInvalidSocket);
  }
  return *this;
}

Socket::~Socket() { close(); }

std::optional<Socket> Socket::connect(std::string_view address, uint16_t port)
{
#ifdef _WIN32
  initializeWinsock();
#endif

  const NativeSocket fd = ::socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == kInvalidSocket) {
    return std::nullopt;
  }

  sockaddr_in addr{
      .sin_family = AF_INET,
      .sin_port = htons(port),
  };

  if (::inet_pton(AF_INET, std::string{address}.c_str(), &addr.sin_addr) != 1) {
    socketClose(fd);
    return std::nullopt;
  }

  if (::bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
    socketClose(fd);
    return std::nullopt;
  }

  return Socket{fd, std::string{address}, port};
}

void Socket::close() noexcept
{
  if (m_fd != kInvalidSocket) {
    socketClose(m_fd);
    m_fd = kInvalidSocket;
  }
}

std::optional<std::size_t> Socket::sendTo(std::span<const std::byte> data, const Peer &peer)
{
  if (!is_open()) {
    return std::nullopt;
  }

  sockaddr_in destination{
      .sin_family = AF_INET,
      .sin_port = htons(peer.port),
  };

  if (::inet_pton(AF_INET, peer.address.c_str(), &destination.sin_addr) != 1) {
    return std::nullopt;
  }

  const int sent = ::sendto(m_fd, reinterpret_cast<const char *>(data.data()),
                            static_cast<int>(data.size()), 0,
                            reinterpret_cast<const sockaddr *>(&destination),
                            sizeof(destination));
  if (sent < 0) {
    return std::nullopt;
  }

  return static_cast<std::size_t>(sent);
}

std::optional<Socket::Datagram> Socket::receiveFrom(std::span<std::byte> buffer)
{
  if (!is_open()) {
    return std::nullopt;
  }

  sockaddr_in sender{};
  socklen_t senderLength = sizeof(sender);
  const int received = ::recvfrom(m_fd, reinterpret_cast<char *>(buffer.data()),
                                  static_cast<int>(buffer.size()), 0,
                                  reinterpret_cast<sockaddr *>(&sender), &senderLength);
  if (received < 0) {
    return std::nullopt;
  }
  if (received == 0) {
    return std::nullopt;
  }

  char address[INET_ADDRSTRLEN]{};
  if (::inet_ntop(AF_INET, &sender.sin_addr, address, sizeof(address)) == nullptr) {
    return std::nullopt;
  }

  return Datagram{static_cast<std::size_t>(received),
                  Peer{address, ntohs(sender.sin_port)}};
}
