#include "Application/UserSessionManager.hpp"

std::shared_ptr<UserSession> UserSessionManager::getOrCreate(const std::string& sessionId)
{
    std::lock_guard lock(m_mutex);
    auto it = m_sessions.find(sessionId);
    if (it != m_sessions.end()) {
        return it->second;
    }

    auto session = std::make_shared<UserSession>();
    session->sessionId = sessionId;
    m_sessions.emplace(sessionId, session);
    return session;
}

std::shared_ptr<UserSession> UserSessionManager::find(const std::string& sessionId)
{
    std::lock_guard lock(m_mutex);
    auto it = m_sessions.find(sessionId);
    if (it == m_sessions.end()) {
        return nullptr;
    }
    return it->second;
}

void UserSessionManager::remove(const std::string& sessionId)
{
    std::lock_guard lock(m_mutex);
    m_sessions.erase(sessionId);
}
