#pragma once

#include <atomic>
#include <cstdint>
#include <string>
#include <string_view>

#include "Application/DEXDecode.hpp"
#include "Application/UserSessionManager.hpp"
#include "Infrastructure/Socket.hpp"
#include "Infrastructure/ThreadPool.hpp"

/**
 * @brief Main application server for the movie booking service.
 *
 * The application manages the UDP socket, decodes requests, keeps user session
 * state and dispatches command processing to a background worker pool.
 */
class Application {
public:
    /**
     * @brief Creates the server and binds it to the configured UDP endpoint.
     * @param port Listening port for incoming packets.
     * @param host Address used to bind the socket.
     */
    explicit Application(uint16_t port = 9000, std::string_view host = "127.0.0.1");
    ~Application();

    /**
     * @brief Starts the receive loop and keeps the service alive.
     * @return 0 while the service exits cleanly; 1 if the socket is not ready.
     */
    int run();

private:
    /**
     * @brief Parses one received datagram and runs the corresponding command.
     * @param payload Raw payload sent by the client.
     * @param peer Client information used to answer back.
     */
    void processDatagram(const std::string& payload, const Socket::Peer& peer);

    /**
     * @brief Sends a textual response back to a client.
     * @param peer Destination address.
     * @param payload Response payload to send.
     */
    void sendResponse(const Socket::Peer& peer, std::string_view payload);

    std::string m_host;          ///< Bind address used by the UDP socket.
    uint16_t m_port{};           ///< Port used by the UDP listener.
    Socket m_socket;            ///< UDP socket used to receive and send datagrams.
    ThreadPool m_pool;          ///< Worker pool used for asynchronous command processing.
    UserSessionManager m_sessions; ///< Per-client session state.
    DEXDecode m_decoder;        ///< Command parser and operation factory.
    std::atomic_bool m_running{true}; ///< Service lifecycle flag.
};
