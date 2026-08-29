#include <array>
#include <iostream>
#include <string>

#include "AddMovie.hpp"
#include "GetAllMovies.hpp"

void expect(bool condition, const std::string& message)
{
    if (!condition)
    {
        throw std::runtime_error(message);
    }
}

void db_basicAddGetOperations()
{
    const std::string expectedName = "movie-id-1";
    auto movie = _MovieScreening::create(
        expectedName,
        expectedName,
        expectedName,
        expectedName);

    expect(AddMovie(std::move(movie)).Execute() == DatabaseError::OK,
           "Fail to add data to database");

    std::array<MovieScreeningType, 20> movies{};
    GetAllMovies getAll(movies);

    expect(getAll.Execute() == DatabaseError::OK,
           "Fail to get all movies database");

    expect(movies[0] != nullptr && movies[0]->movie_name == expectedName,
           "Movie name differ from expected");
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
    run("invalid bookings", db_basicAddGetOperations, failures);
    return failures == 0 ? 0 : 1;
}