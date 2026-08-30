#include "../tests.hpp"

#include <array>
#include <string>
#include <vector>

#include "Application/DEXDecode.hpp"
#include "Application/UserSessionManager.hpp"
#include "MovieSessionDatabase/MovieSessionDatabase.hpp"
#include "MovieSessionVisitor.hpp"
#include "AddMovie.hpp"
#include "GetAllAvailableSeatsForMovieAndTheater.hpp"

void session_manager_tracks_client_state()
{
    UserSessionManager manager;

    auto sessionA = manager.getOrCreate("client-1");
    sessionA->request.selectedMovie = "Inception";
    sessionA->request.selectedTheater = "Cinema 1";

    auto sessionB = manager.find("client-1");
    expect(sessionB != nullptr, "Session should be available after creation");
    expect(sessionA == sessionB, "Same session id should return the same session instance");
    expect(sessionB->request.selectedMovie == "Inception", "Movie should be preserved in user session");
    expect(sessionB->request.selectedTheater == "Cinema 1", "Theater should be preserved in user session");
}

void decode_select_movie_updates_user_session()
{
    //populate db
    (void)
    AddMovie(MovieScreening::create(
        std::string("Inception"),
        std::string("IMAX")
    )).Execute();

    DEXDecode decoder;
    const auto command = decoder.decode("client-2 SELECT_MOVIE Inception");

    expect(command.has_value(), "SELECT_MOVIE should decode successfully");

    auto operation = decoder.makeOperation(*command);
    expect(operation.has_value(), "Decoded command should produce an operation");

    UserSession session{.sessionId = "client-2"};
    const auto response = (*operation)(session);
    std::cout << response << std::endl;
    expect(response == "THEATERS:IMAX", "SELECT_MOVIE operation should return the list of theaters showing the movie");
    expect(session.request.selectedMovie == "Inception", "Movie should be stored in the session");
}

void decode_book_command_updates_database_and_session()
{
    auto addMovie = MovieScreening::create("Inception", "Cinema 1");
    AddMovie addOperation(addMovie);
    expect(addOperation.Execute() == DatabaseError::OK, "The movie should be added to the static database before booking");

    DEXDecode decoder;
    const auto command = decoder.decode("client-3 BOOK A1 A2");
    expect(command.has_value(), "BOOK command should decode successfully");

    auto operation = decoder.makeOperation(*command);
    expect(operation.has_value(), "BOOK command should produce an operation");

    UserSession session{.sessionId = "client-3"};
    session.request.selectedMovie = "Inception";
    session.request.selectedTheater = "Cinema 1";

    const auto response = (*operation)(session);
    expect(response == "BOOKED", "Booking operation should confirm a successful reservation");
    expect(session.request.selectedSeats.size() == 2, "The selected seats should be recorded in the session");

    std::array<std::string, 20> seats{};
    GetAllAvailableSeatsForMovieAndTheater query{"Inception", "Cinema 1", seats};
    expect(query.Execute() == DatabaseError::OK, "Seat query should still be valid after booking");
    expect(std::find(seats.begin(), seats.end(), "A1") == seats.end(), "Booked seat A1 should no longer be available");
    expect(std::find(seats.begin(), seats.end(), "A2") == seats.end(), "Booked seat A2 should no longer be available");
}

int main()
{
    int failures = 0;

    run("session manager tracks client state", session_manager_tracks_client_state, failures);
    run("decode select movie updates user session", decode_select_movie_updates_user_session, failures);
    run("decode book command updates database and session", decode_book_command_updates_database_and_session, failures);

    return failures == 0 ? 0 : 1;
}
