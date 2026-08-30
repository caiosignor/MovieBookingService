#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "Application/UserSessionManager.hpp"

enum class CommandType {
    GetMovies,
    SelectMovie,
    GetTheaters,
    SelectTheater,
    GetSeats,
    Book
};

struct DEXCommand {
    std::string session_id;
    CommandType type;
    std::vector<std::string> args;
};

enum class DecodeError {
    InvalidFormat,
    UnknownCommand,
    MissingArguments
};

class DEXDecode {
public:
    DEXDecode();
    ~DEXDecode();

    std::optional<DEXCommand> decode(std::string_view commandStr);
    std::vector<std::string> decodeDEX(std::string_view commands);
    std::optional<std::function<std::string(UserSession&)>> makeOperation(const DEXCommand& command) const;
};