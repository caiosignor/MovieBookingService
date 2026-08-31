#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "Application/UserSessionManager.hpp"

/**
 * @brief Available command types supported by the protocol.
 */
enum class CommandType {
    GetMovies,      ///< Lists all movies currently available.
    SelectMovie,    ///< Selects a movie for the current session.
    SelectTheater,  ///< Selects a theater for the chosen movie.
    Book            ///< Books one or more seats.
};

/**
 * @brief Parsed command with the session id and arguments.
 */
struct DEXCommand {
    std::string session_id;      ///< Client session identifier.
    CommandType type;           ///< Parsed command type.
    std::vector<std::string> args; ///< Additional command arguments.
};

/**
 * @brief Error states that can happen during decode validation.
 */
enum class DecodeError {
    InvalidFormat,   ///< Message does not follow the expected format.
    UnknownCommand,  ///< Command name is not recognized.
    MissingArguments ///< Required arguments are missing.
};

/**
 * @brief Parses the text protocol and converts it into database operations.
 *
 * The class receives a raw DEX string, splits it into tokens and builds the
 * proper operation for the user session.
 */
class DEXDecode {
public:
    DEXDecode();
    ~DEXDecode();

    /**
     * @brief Converts a command string into a structured command.
     * @param commandStr Input from the client.
     * @return Parsed command when the format is valid; nullopt otherwise.
     */
    std::optional<DEXCommand> decode(std::string_view commandStr);

    /**
     * @brief Splits a command text into token pieces.
     * @param commands Raw input string.
     * @return Sequence of words and arguments.
     */
    std::vector<std::string> decodeDEX(std::string_view commands);

    /**
     * @brief Builds the lambda that executes the requested operation.
     * @param command Parsed command.
     * @return Function that acts on a session and returns the response string.
     */
    std::optional<std::function<std::string(UserSession&)>> makeOperation(const DEXCommand& command) const;
};