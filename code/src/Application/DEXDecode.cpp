#include "DEXDecode.hpp"
#include <sstream>

DEXDecode::DEXDecode() {}
DEXDecode::~DEXDecode() {}

std::expected<DEXCommand, DecodeError> DEXDecode::decode(std::string_view commandStr)
{
    auto tokens = decodeDEX(commandStr);
    if (tokens.size() < 2) {
        return std::unexpected(DecodeError::InvalidFormat);
    }

    std::string sessionId = tokens[0];
    std::string commandName = tokens[1];
    CommandType type;

    if (commandName == "GET_MOVIES") {
        type = CommandType::GetMovies;
    } else if (commandName == "SELECT_MOVIE") {
        type = CommandType::SelectMovie;
    } else if (commandName == "GET_THEATERS") {
        type = CommandType::GetTheaters;
    } else if (commandName == "SELECT_THEATER") {
        type = CommandType::SelectTheater;
    } else if (commandName == "GET_SEATS") {
        type = CommandType::GetSeats;
    } else if (commandName == "BOOK") {
        type = CommandType::Book;
    } else {
        return std::unexpected(DecodeError::UnknownCommand);
    }

    if ((type == CommandType::SelectMovie || type == CommandType::SelectTheater || type == CommandType::Book) && tokens.size() < 3) {
        return std::unexpected(DecodeError::MissingArguments);
    }

    std::vector<std::string> args;
    for (size_t i = 2; i < tokens.size(); ++i) {
        args.push_back(tokens[i]);
    }

    return DEXCommand{sessionId, type, args};
}

std::vector<std::string> DEXDecode::decodeDEX(std::string_view commands)
{
    std::istringstream input{std::string(commands)};
    std::vector<std::string> result;
    for (std::string token; input >> token;) result.push_back(std::move(token));
    return result;
}