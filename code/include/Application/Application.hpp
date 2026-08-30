#pragma once

#include <atomic>
#include <cstdint>
#include <string>
#include <string_view>

#include "Application/DEXDecode.hpp"
#include "Application/UserSessionManager.hpp"
#include "Infrastructure/Socket.hpp"
#include "Infrastructure/ThreadPool.hpp"

class Application {
public:
    explicit Application(uint16_t port = 9000, std::string_view host = "127.0.0.1");
    ~Application();

    int run();

private:
    void processDatagram(const std::string& payload, const Socket::Peer& peer);
    void sendResponse(const Socket::Peer& peer, std::string_view payload);

    std::string m_host;
    uint16_t m_port{};
    Socket m_socket;
    ThreadPool m_pool;
    UserSessionManager m_sessions;
    DEXDecode m_decoder;
    std::atomic_bool m_running{true};
};
