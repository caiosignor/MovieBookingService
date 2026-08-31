#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Stores the current booking state for a user.
 */
struct UserRequest {
    std::string selectedMovie;    ///< Movie chosen by the user.
    std::string selectedTheater;  ///< Theater selected for that movie.
    std::vector<std::string> selectedSeats; ///< Seats requested for booking.
};

/**
 * @brief Session data associated with one client connection.
 */
struct UserSession {
    std::string sessionId; ///< Unique session identifier.
    UserRequest request;   ///< State accumulated during the booking flow.
};

/**
 * @brief Manages user sessions with thread-safe access.
 *
 * The manager keeps one session per client id and allows lookup and removal in
 * a synchronized way.
 */
class UserSessionManager {
public:
    UserSessionManager() = default;

    /**
     * @brief Returns an existing session or creates a new one.
     * @param sessionId Client identifier.
     * @return Shared pointer to the session.
     */
    std::shared_ptr<UserSession> getOrCreate(const std::string& sessionId);

    /**
     * @brief Finds an existing session by id.
     * @param sessionId Client identifier.
     * @return Session pointer or nullptr if not found.
     */
    std::shared_ptr<UserSession> find(const std::string& sessionId);

    /**
     * @brief Removes an existing session from the map.
     * @param sessionId Client identifier.
     */
    void remove(const std::string& sessionId);

private:
    std::mutex m_mutex; ///< Protects access to the session table.
    std::unordered_map<std::string, std::shared_ptr<UserSession>> m_sessions; ///< Active sessions.
};
