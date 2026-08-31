#include "Application/DEXDecode.hpp"

#include <sstream>
#include <string>
#include <vector>

#include "BookSeatForMovieAndTheater.hpp"
#include "GetAllAvailableSeatsForMovieAndTheater.hpp"
#include "GetAllMovies.hpp"
#include "GetAllTheaterShowingTheMovie.hpp"

namespace {
std::string joinArgs(const std::vector<std::string>& values)
{
    std::string result;
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) {
            result += ' ';
        }
        result += values[i];
    }
    return result;
}

std::string joinSeats(std::span<const std::string> values)
{
    std::string result;
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) {
            result += '|';
        }
        result += values[i];
    }
    return result;
}

std::string joinMovies(std::span<const MovieScreening> movies)
{
    std::string result;
    for (std::size_t i = 0; i < movies.size(); ++i) {
        if (i != 0) {
            result += '|';
        }
        result += movies[i].movie_name;
    }
    return result;
}

std::string joinTheaters(std::span<const std::string> Theaters)
{
    std::string result;
    if(!Theaters[0].empty())
    {
        result = Theaters[0];
    }

    for (std::size_t i = 1; i < Theaters.size(); ++i) {
        if (!Theaters[i].empty()) {
            result += "| ";
            result += Theaters[i];
        }
    }
    return result;
}
} // namespace

DEXDecode::DEXDecode() {}
DEXDecode::~DEXDecode() {}

std::optional<DEXCommand> DEXDecode::decode(std::string_view commandStr)
{
    // Split the message into tokens to validate the protocol format.
    auto tokens = decodeDEX(commandStr);
    if (tokens.size() < 2) {
        return std::nullopt;
    }

    const std::string sessionId = tokens[0];
    const std::string commandName = tokens[1];
    CommandType type{};

    if (commandName == "GET_MOVIES") {
        type = CommandType::GetMovies;
    } else if (commandName == "SELECT_MOVIE") {
        type = CommandType::SelectMovie;
    } else if (commandName == "SELECT_THEATER") {
        type = CommandType::SelectTheater;
    } else if (commandName == "BOOK") {
        type = CommandType::Book;
    } else {
        return std::nullopt;
    }

    // Commands that require arguments must include at least one extra value.
    if ((type == CommandType::SelectMovie || type == CommandType::SelectTheater || type == CommandType::Book) && tokens.size() < 3) {
        return std::nullopt;
    }

    std::vector<std::string> args;
    for (std::size_t i = 2; i < tokens.size(); ++i) {
        args.push_back(tokens[i]);
    }

    return DEXCommand{sessionId, type, args};
}

std::vector<std::string> DEXDecode::decodeDEX(std::string_view commands)
{
    std::istringstream input{std::string(commands)};
    std::vector<std::string> result;
    for (std::string token; input >> token;) {
        result.push_back(std::move(token));
    }
    return result;
}

std::optional<std::function<std::string(UserSession&)>> DEXDecode::makeOperation(const DEXCommand& command) const
{
    switch (command.type) {
    case CommandType::GetMovies: {
        return [command](UserSession&) -> std::string {
            // Query the catalog of currently available movies.
            std::vector<MovieScreening> movies(64);
            GetAllMovies op{std::span<MovieScreening>(movies)};
            if (op.Execute() != DatabaseError::OK) {
                return "ERROR:GET_MOVIES";
            }
            return "MOVIES:" + joinMovies(std::span<const MovieScreening>(movies));
        };
    }
    case CommandType::SelectMovie: {
        const std::string movieName = joinArgs(command.args);
        return [movieName](UserSession& session) -> std::string {
            // Find all theaters currently showing the selected movie.
            std::vector<std::string> theaters(64);
            GetAllTheaterShowingTheMovie op{movieName, std::span<std::string>(theaters)};
            if (op.Execute() != DatabaseError::OK) {
                return "ERROR:MOVIE IS NOT ON SCREEN";
            }
            session.request.selectedMovie = movieName;
            return "THEATERS:" + joinTheaters(std::span<const std::string>(theaters));
        };
    }
    case CommandType::SelectTheater: {
        const std::string theaterName = joinArgs(command.args);
        return [theaterName](UserSession& session) -> std::string {
            // After selecting the theater, look up the free seats for the current movie.
            std::vector<std::string> seats(64);
            GetAllAvailableSeatsForMovieAndTheater op{session.request.selectedMovie,
                                                      theaterName,
                                                      std::span<std::string>(seats)};
            if (op.Execute() != DatabaseError::OK) {
                return "ERROR:GET_SEATS";
            }
            session.request.selectedSeats.clear();
            session.request.selectedTheater = theaterName;
            return "SEATS:" + joinSeats(std::span<const std::string>(seats));
        };
    }
    case CommandType::Book: {
        const std::vector<std::string> requestedSeats = command.args;
        return [requestedSeats](UserSession& session) -> std::string {
            // Save the current request and try to reserve the selected seats.
            session.request.selectedSeats = requestedSeats;

            std::vector<std::string_view> seats;
            seats.reserve(requestedSeats.size());
            for (const auto& seat : requestedSeats) {
                seats.emplace_back(seat);
            }

            BookSeatForMovieAndTheater op{session.request.selectedMovie,
                                          session.request.selectedTheater,
                                          std::span<const std::string_view>(seats)};
            return op.Execute() == DatabaseError::OK ? "BOOKED" : "ERROR:SEAT_UNAVAILABLE";
        };
    }
    }

    return std::nullopt;
}
