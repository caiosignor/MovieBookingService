#include "infrastructure/Socket.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

Socket::Socket(int fd, std::string address, uint16_t port) noexcept
    : m_address{std::move(address)}, m_port{port}, m_fd{fd} {}

Socket::Socket(Socket &&other) noexcept
    : m_address{std::move(other.m_address)}, m_port{other.m_port},
      m_fd{std::exchange(other.m_fd, -1)} {}

Socket &Socket::operator=(Socket &&other) noexcept
{
  if (this != &other) {
    close();
    m_address = std::move(other.m_address);
    m_port = other.m_port;
    m_fd = std::exchange(other.m_fd, -1);
  }
  return *this;
}

Socket::~Socket() { close(); }

std::expected<Socket, Socket::Error> Socket::connect(std::string_view address,
                                                     uint16_t port)
{
  const int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    return std::unexpected(Error::CreationFailed);
  }

  sockaddr_in addr{
      .sin_family = AF_INET,
      .sin_port = htons(port),
  };

  if (::bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
    ::close(fd);
    return std::unexpected(Error::BindFailed);
  }

  return Socket{fd, std::string{address}, port};
}

void Socket::close() noexcept
{
  if (m_fd >= 0) {
    ::close(m_fd);
    m_fd = -1;
  }
}

std::expected<std::size_t, Socket::Error>
Socket::sendTo(std::span<const std::byte> data, const Peer &peer)
{
  if (!is_open()) {
    return std::unexpected(Error::NotConnected);
  }

  sockaddr_in destination{
      .sin_family = AF_INET,
      .sin_port = htons(peer.port),
  };

  if (::inet_pton(AF_INET, peer.address.c_str(), &destination.sin_addr) != 1) {
    return std::unexpected(Error::SendFailed);
  }

  const ssize_t sent = ::sendto(m_fd, data.data(), data.size(), MSG_NOSIGNAL,
                                reinterpret_cast<const sockaddr *>(&destination),
                                sizeof(destination));
  if (sent < 0) {
    return std::unexpected(Error::SendFailed);
  }

  return static_cast<std::size_t>(sent);
}

std::expected<Socket::Datagram, Socket::Error>
Socket::receiveFrom(std::span<std::byte> buffer)
{
  if (!is_open()) {
    return std::unexpected(Error::NotConnected);
  }

  sockaddr_in sender{};
  socklen_t senderLength = sizeof(sender);
  const ssize_t received = ::recvfrom(m_fd, buffer.data(), buffer.size(), 0,
                                      reinterpret_cast<sockaddr *>(&sender), &senderLength);
  if (received < 0) {
    return std::unexpected(Error::ReceiveFailed);
  }
  if (received == 0) {
    return std::unexpected(Error::ConnectionClosed);
  }

  char address[INET_ADDRSTRLEN]{};
  if (::inet_ntop(AF_INET, &sender.sin_addr, address, sizeof(address)) == nullptr) {
    return std::unexpected(Error::ReceiveFailed);
  }

  return Datagram{static_cast<std::size_t>(received),
                  Peer{address, ntohs(sender.sin_port)}};
}
