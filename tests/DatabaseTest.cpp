#include <array>
#include <iostream>
#include <string>
#include <ranges>
#include "AddMovie.hpp"
#include "GetAllMovies.hpp"

void expect(bool condition, const std::string& message)
{
    if (!condition)
    {
        throw std::runtime_error(message);
    }
}

void db_addAndGetMovie()
{
    constexpr std::string_view movieId = "movie-id-1";
    constexpr std::string_view theaterId = "theater-id-1";
    constexpr std::string_view movieName = "The Dark Knight";
    constexpr std::string_view theaterName = "Cineplex";

    auto movie = MovieScreening::create(
        std::string{movieId},
        std::string{theaterId},
        std::string{movieName},
        std::string{theaterName});

    expect(movie->movie_id == "movie-id-1",
       "Movie ID is incorrect after creation");

    expect(movie->theater_id == "theater-id-1",
        "Theater ID is incorrect after creation");

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

    expect(result.movie_id == movieId,
           "Movie ID differs from expected");

    expect(result.theater_id == theaterId,
           "Theater ID differs from expected");

    expect(result.movie_name == movieName,
           "Movie name differs from expected");

    expect(result.theater_name == theaterName,
           "Theater name differs from expected");
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
    run("invalid bookings", db_addAndGetMovie, failures);
    return failures == 0 ? 0 : 1;
}