#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

struct UserRequest {
    std::string selectedMovie;
    std::string selectedTheater;
    std::vector<std::string> selectedSeats;
};

struct UserSession {
    std::string sessionId;
    UserRequest request;
};

class UserSessionManager {
public:
    UserSessionManager() = default;

    std::shared_ptr<UserSession> getOrCreate(const std::string& sessionId);
    std::shared_ptr<UserSession> find(const std::string& sessionId);
    void remove(const std::string& sessionId);

private:
    std::mutex m_mutex;
    std::unordered_map<std::string, std::shared_ptr<UserSession>> m_sessions;
};
