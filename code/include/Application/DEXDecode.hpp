#pragma once

#include <string>
#include <vector>
#include <expected>
#include <string_view>

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

    std::expected<DEXCommand, DecodeError> decode(std::string_view commandStr);
    std::vector<std::string> decodeDEX(std::string_view commands);
};