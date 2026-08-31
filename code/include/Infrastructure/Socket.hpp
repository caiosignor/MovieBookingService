#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>

/**
 * @brief Minimal UDP socket wrapper used by the booking service.
 */
class Socket {
public:
  /**
   * @brief Remote endpoint information.
   */
  struct Peer {
    std::string address; ///< IP address or hostname string.
    uint16_t port{};     ///< Remote port.
  };

  /**
   * @brief Packet received from a peer.
   */
  struct Datagram {
    std::size_t size{}; ///< Number of bytes received.
    Peer peer;          ///< Originating client.
  };

  /**
   * @brief Socket error codes used by the wrapper.
   */
  enum class Error {
    CreationFailed,
    BindFailed,
    SendFailed,
    ReceiveFailed,
    ConnectionClosed,
    NotConnected,
  };

  /**
   * @brief Converts an error code to a readable string.
   * @param error Error value.
   * @return Text description of the code.
   */
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

  /**
   * @brief Opens a UDP socket and binds it to a local endpoint.
   * @param address Local address to bind.
   * @param port Local port to listen on.
   * @return Ready socket when creation succeeds.
   */
  [[nodiscard]] static std::optional<Socket>
  connect(std::string_view address, uint16_t port);

  /**
   * @brief Closes the socket if it is currently open.
   */
  void close() noexcept;

  [[nodiscard]] bool is_open() const noexcept { return m_fd >= 0; }
  [[nodiscard]] std::string_view address() const noexcept { return m_address; }
  [[nodiscard]] uint16_t port() const noexcept { return m_port; }

  /**
   * @brief Sends a payload to a given remote peer.
   * @param data Bytes to send.
   * @param peer Destination address.
   * @return Number of bytes sent when successful.
   */
  [[nodiscard]] std::optional<std::size_t>
  sendTo(std::span<const std::byte> data, const Peer &peer);

  /**
   * @brief Receives one datagram into a provided buffer.
   * @param buffer Destination buffer.
   * @return Datagram information when a packet is received.
   */
  [[nodiscard]] std::optional<Datagram>
  receiveFrom(std::span<std::byte> buffer);

private:
  Socket(int fd, std::string address, uint16_t port) noexcept;

  std::string m_address; ///< Bound local address.
  uint16_t m_port{};     ///< Bound local port.
  int m_fd{-1};          ///< Native socket descriptor.
};
