#include "../tests.hpp"
#include "DEXDecode.hpp"

void decode_accepts_valid_get_movies_request()
{
    DEXDecode decoder;
    auto result = decoder.decode("session-1 GET_MOVIES");

    expect(result.has_value(), "GET_MOVIES should be decoded successfully");
    expect(result->session_id == "session-1", "Session id should be preserved");
    expect(result->type == CommandType::GetMovies, "Command type should be GetMovies");
    expect(result->args.empty(), "GET_MOVIES should not contain arguments");
}

void decode_parses_select_movie_with_arguments()
{
    DEXDecode decoder;
    auto result = decoder.decode("session-2 SELECT_MOVIE The Dark Knight");

    expect(result.has_value(), "SELECT_MOVIE should be parsed successfully");
    expect(result->type == CommandType::SelectMovie, "Command type should be SelectMovie");
    expect(result->args.size() == 3, "Movie selection should include all title word tokens as arguments");
    expect(result->args[0] == "The", "First movie argument should be 'The'");
    expect(result->args[1] == "Dark", "Second movie argument should be 'Dark'");
    expect(result->args[2] == "Knight", "Third movie argument should be 'Knight'");
}

void decode_rejects_unknown_command()
{
    DEXDecode decoder;
    auto result = decoder.decode("session-3 UNKNOWN_COMMAND");

    expect(!result.has_value(), "Unknown commands should be rejected");
    expect(result.error() == DecodeError::UnknownCommand, "Unknown command should return UnknownCommand");
}

void decode_requires_arguments_for_select_movie()
{
    DEXDecode decoder;
    auto result = decoder.decode("session-4 SELECT_MOVIE");

    expect(!result.has_value(), "SELECT_MOVIE without arguments should be rejected");
    expect(result.error() == DecodeError::MissingArguments, "Missing arguments should be reported as MissingArguments");
}

int main()
{
    int failures = 0;

    run("decode accepts valid GET_MOVIES request", decode_accepts_valid_get_movies_request, failures);
    run("decode parses SELECT_MOVIE with arguments", decode_parses_select_movie_with_arguments, failures);
    run("decode rejects unknown commands", decode_rejects_unknown_command, failures);
    run("decode requires arguments for SELECT_MOVIE", decode_requires_arguments_for_select_movie, failures);

    return failures == 0 ? 0 : 1;
}