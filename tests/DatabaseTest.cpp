#include <array>
#include <iostream>
#include <string>
#include <ranges>
#include <format>
#include <algorithm>
#include "AddMovie.hpp"
#include "GetAllMovies.hpp"
#include "GetAllTheaterShowingTheMovie.hpp"
#include "GetAllAvailableSeatsForMovieAndTheater.hpp"
#include "BookSeatForMovieAndTheater.hpp"
#include "DatabaseTestData.hpp"


void expect(bool condition, const std::string& message)
{
    if (!condition)
    {
        throw std::runtime_error(message);
    }
}

void db_addAndGetMovie()
{
    constexpr std::string_view movieName = "The Dark Knight";
    constexpr std::string_view theaterName = "Cineplex";

    auto movie = MovieScreening::create(
        std::string{movieName},
        std::string{theaterName});

    expect(movie->movie_name == "The Dark Knight",
        "Movie name is incorrect after creation");

    expect(movie->theater_name == "Cineplex",
        "Theater name is incorrect after creation");

    expect(movie != nullptr,
           "Failed to create movie");

    expect(
        AddMovie(std::move(movie)).Execute() == DatabaseError::OK,
        "Failed to add movie");

    std::array<MovieScreening, 20> movies{};

    GetAllMovies getAll{movies};

    expect(
        getAll.Execute() == DatabaseError::OK,
        "Failed to get movies");

    const auto result = movies[0];

    expect(result.movie_name == movieName,
           "Movie name differs from expected");

    expect(result.theater_name == theaterName,
           "Theater name differs from expected");
}

void db_GetAllTheaterShowingTheMovie()
{
    constexpr std::string_view movieName = "The Dark Knight";
    constexpr std::string_view theaterName = "Cineplex";

    std::array<std::string, 20> theaters;
    expect(GetAllTheaterShowingTheMovie(movieName, theaters).Execute() == DatabaseError::OK, "Not found a theater");
    
    const auto result = theaters[0];
    expect(result == theaterName, std::format("Theather Name differ from expected. Expected({}) Found({})", theaterName, result));

}

void db_getAllAvailableSeats()
{
    constexpr std::string_view movieName = "The Dark Knight";
    constexpr std::string_view theaterName = "Cineplex";

    auto movie = MovieScreening::create(
        std::string{movieName},
        std::string{theaterName});

    // Adiciona movie à database...

    std::array<std::string, 20> seats{};

    GetAllAvailableSeatsForMovieAndTheater operation{
        movieName,
        theaterName,
        seats};

    expect(
        operation.Execute() == DatabaseError::OK,
        "Failed to get available seats");

    auto containsSeat = [&](std::string_view expected)
    {
        return std::ranges::any_of(
            seats,
            [expected](const auto& seat)
            {
                return seat == expected;
            });
    }
    ;
    for (const auto& [seat, available] : movie->m_availableSeats)
    {
        expect(containsSeat(seat), "Test setup contains unavailable seat");
    }

}

void db_bookaSeat()
{
    constexpr std::string_view movieName = "The Dark Knight";
    constexpr std::string_view theaterName = "Cineplex";

    expect(BookSeatForMovieAndTheater(movieName, theaterName, TestData::RequestedSeats).Execute() == DatabaseError::OK, "Failed to book a seat");
    expect(BookSeatForMovieAndTheater(movieName, theaterName, TestData::RequestedSeats).Execute() == DatabaseError::SeatUnavailable, "Seat should be unavailable at this stage. Already occupied by previous step");
}

void run(const char* name, void (*test)(), int& failures)
{
    try
    {
        test();
        std::cout << "[PASS] " << name << '\n';
    }
    catch (const std::exception& error)
    {
        ++failures;
        std::cerr << "[FAIL] " << name << ": " << error.what() << '\n';
    }
}

int main()
{
    int failures = 0;
    run("Simple add and get operations", db_addAndGetMovie, failures);
    run("Getting all theaters showing the movie", db_GetAllTheaterShowingTheMovie, failures);
    run("Getting all available seats for the movie", db_getAllAvailableSeats, failures);
    run("Book a seat", db_bookaSeat, failures);
    return failures == 0 ? 0 : 1;
}