#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>
#include <string>
#include <string_view>

class Socket {
public:
  struct Peer {
    std::string address;
    uint16_t port{};
  };

  struct Datagram {
    std::size_t size{};
    Peer peer;
  };

  enum class Error {
    CreationFailed,
    BindFailed,
    SendFailed,
    ReceiveFailed,
    ConnectionClosed,
    NotConnected,
  };

  [[nodiscard]]
  constexpr static std::string_view toString(Error error) noexcept
  {
    switch (error) {
    case Error::CreationFailed:
        return "CreationFailed";

    case Error::BindFailed:
        return "BindFailed";

    case Error::SendFailed:
        return "SendFailed";

    case Error::ReceiveFailed:
        return "ReceiveFailed";

    case Error::ConnectionClosed:
        return "ConnectionClosed";

    case Error::NotConnected:
        return "NotConnected";
    }

    return "Unknown";
  }

  Socket() = default;
  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;
  Socket(Socket &&other) noexcept;
  Socket &operator=(Socket &&other) noexcept;
  ~Socket();

  [[nodiscard]] static std::expected<Socket, Error>
  connect(std::string_view address, uint16_t port);

  void close() noexcept;

  [[nodiscard]] bool is_open() const noexcept { return m_fd >= 0; }
  [[nodiscard]] std::string_view address() const noexcept { return m_address; }
  [[nodiscard]] uint16_t port() const noexcept { return m_port; }

  [[nodiscard]] std::expected<std::size_t, Error>
  sendTo(std::span<const std::byte> data, const Peer &peer);

  [[nodiscard]] std::expected<Datagram, Error>
  receiveFrom(std::span<std::byte> buffer);

private:
  Socket(int fd, std::string address, uint16_t port) noexcept;

  std::string m_address;
  uint16_t m_port{};
  int m_fd{-1};
};
