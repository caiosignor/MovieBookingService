#include "Application/Application.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string_view>
#include "AddMovie.hpp"

namespace {
std::string trim(std::string_view value)
{
    const auto begin = value.find_first_not_of(" \t\r\n");
    if (begin == std::string_view::npos) {
        return {};
    }

    const auto end = value.find_last_not_of(" \t\r\n");
    return std::string(value.substr(begin, end - begin + 1));
}
} // namespace

Application::Application(uint16_t port, std::string_view host)
    : m_host(host),
      m_port(port),
      m_pool(std::max<std::size_t>(1U, std::thread::hardware_concurrency()))
{
    // Open the UDP socket for the server using the configured address.
    auto socketResult = Socket::connect(m_host, m_port);
    if (!socketResult) {
        throw std::runtime_error("Failed to bind UDP socket for the application.");
    }
    m_socket = std::move(const_cast<Socket&>(*socketResult));

    // Seed the application database with sample movies.
    constexpr auto Movies = std::to_array({
        std::pair{std::string_view{"The Dark Knight"}, std::string_view{"Cineplex"}},
        std::pair{std::string_view{"The Dark Knight"}, std::string_view{"IMAX"}},
        std::pair{std::string_view{"Inception"}, std::string_view{"IMAX"}},
        std::pair{std::string_view{"Interstellar"}, std::string_view{"Cinemark"}},
        std::pair{std::string_view{"The Matrix"}, std::string_view{"AMC"}},
        std::pair{std::string_view{"Pulp Fiction"}, std::string_view{"Regal"}}
    });

    for (auto m : Movies) {
        // Insert the sample movies into the in-memory database.
        (void)AddMovie(MovieScreening::create(std::string(m.first), std::string(m.second))).Execute();
    }
}

Application::~Application() { m_running = false; }

int Application::run()
{
    if (!m_socket.is_open()) {
        return 1;
    }

    std::array<std::byte, 4096> buffer{};
    while (m_running) {
        // Wait for the next UDP message and handle it immediately after.
        const auto packet = m_socket.receiveFrom(buffer);
        if (!packet) {
            continue;
        }

        const std::string payload(reinterpret_cast<const char*>(buffer.data()), packet->size);
        processDatagram(payload, packet->peer);
    }

    return 0;
}

void Application::processDatagram(const std::string& payload, const Socket::Peer& peer)
{
    // Remove extra whitespace to avoid empty or malformed commands.
    std::string commandText = trim(payload);
    if (commandText.empty()) {
        return;
    }

    const auto decoded = m_decoder.decode(commandText);
    if (!decoded) {
        sendResponse(peer, "ERROR:INVALID_COMMAND");
        return;
    }

    const auto operation = m_decoder.makeOperation(*decoded);
    if (!operation) {
        sendResponse(peer, "ERROR:INVALID_COMMAND");
        return;
    }

    // Keep the client state in a session even when the operation runs in a worker thread.
    auto session = m_sessions.getOrCreate(decoded->session_id);
    const auto task = [this, peer, session, operation = std::move(*operation)]() mutable {
        const auto response = operation(*session);
        sendResponse(peer, response);
    };

    m_pool.enqueue(task);
}

void Application::sendResponse(const Socket::Peer& peer, std::string_view payload)
{
    const auto bytes = std::span<const std::byte>(reinterpret_cast<const std::byte*>(payload.data()), payload.size());
    (void)m_socket.sendTo(bytes, peer);
}
