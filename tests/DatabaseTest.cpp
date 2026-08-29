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

void movieScreeningBuilder()
{
    auto movie = MovieScreening::create(
        std::string{TestData::MovieName},
        std::string{TestData::TheaterName});

    expect(movie != nullptr, "Failed to create movie");

    expect(movie->movie_name.compare(TestData::MovieName) == 0, "Movie name is incorrect after creation");
    expect(movie->theater_name.compare(TestData::TheaterName) == 0, "Theater name is incorrect after creation");
}

void db_addAndGetMovie()
{
    MovieSessionDatabase dbInstance;

    for(auto& [movieName, theaterName] : TestData::Movies)
    {
        auto movie = MovieScreening::create(
            std::string{movieName},
            std::string{theaterName}
        );
        AddMovie operation(movie);
        expect(operation.Execute(dbInstance) == DatabaseError::OK, "Failed to add movie!");
    }

    //get all movies from database
    std::array<MovieScreening, 20> movies{};
    GetAllMovies getAllOperation{movies};
    expect(getAllOperation.Execute() == DatabaseError::OK, "Failed to get movies from database");

    auto firstMovie = movies[0];
    expect(firstMovie.movie_name.compare(TestData::MovieName),
        std::format("Retrieved different movie name from database. Expected{} Current{}",TestData::MovieName, firstMovie.movie_name));

    expect(firstMovie.theater_name.compare(TestData::TheaterName),
        std::format("Retrieved different movie name from database. Expected{} Current{}",TestData::TheaterName, firstMovie.theater_name));

    auto movie = MovieScreening::create(
        "Extra Movie",
        "Extra theater"
    );

    AddMovie operation(movie);
    expect(operation.Execute(dbInstance) == DatabaseError::OutOfMemory, "The software accessed invalid memory on add operation");

    std::array<MovieScreening, 10> smaller_movies_list{};
    expect(GetAllMovies(smaller_movies_list).Execute(dbInstance) == DatabaseError::OutOfMemory, "The software accessed invalid memory on get operation");
}

void db_memoryProtectionWhenAddAndGet()
{
     MovieSessionDatabase dbInstance;

    for(auto& [movieName, theaterName] : TestData::Movies)
    {
        auto movie = MovieScreening::create(
            std::string{movieName},
            std::string{theaterName}
        );
        AddMovie operation(movie);
        expect(operation.Execute(dbInstance) == DatabaseError::OK, "Failed to add movie!");
    }

    auto movie = MovieScreening::create(
        "Extra Movie",
        "Extra theater"
    );

    AddMovie operation(movie);
    expect(operation.Execute(dbInstance) == DatabaseError::OutOfMemory, "The software accessed invalid memory on add operation");

    constexpr size_t movies_count = TestData::Movies.size();

    std::array<MovieScreening, movies_count/2> smaller_movies_list{};
    expect(GetAllMovies(smaller_movies_list).Execute(dbInstance) == DatabaseError::OutOfMemory, "The software accessed invalid memory on get operation");
}

void db_GetAllTheaterShowingTheMovie()
{
    MovieSessionDatabase dbInstance;

    for(auto& [movieName, theaterName] : TestData::Movies)
    {
        auto movie = MovieScreening::create(
            std::string{movieName},
            std::string{theaterName}
        );
        AddMovie operation(movie);
        expect(operation.Execute(dbInstance) == DatabaseError::OK, "Failed to add movie!");
    }

    constexpr size_t numberOfMovies = TestData::Movies.size();
    std::array<std::string, numberOfMovies> theaters;

    expect(GetAllTheaterShowingTheMovie(TestData::MovieName, theaters).Execute(dbInstance) == DatabaseError::OK,
        std::format("Not found a theater showing {}", TestData::MovieName));

    
    int numberOfTheater = std::count_if(theaters.begin(), theaters.end(), [](const std::string& s){
        return !s.empty();
    });
        
    expect(numberOfTheater == 2,
        std::format("there are currently 2 theaters showing {}, number of theaters {}", TestData::MovieName, numberOfTheater));

}

void db_getAllAvailableSeats()
{
    MovieSessionDatabase dbInstance;
    auto movie = MovieScreening::create(
        std::string{TestData::MovieName},
        std::string{TestData::TheaterName});

    AddMovie operation(movie);
    expect(operation.Execute(dbInstance) == DatabaseError::OK, "Failed to add movie!");

    std::array<std::string, 20> seats{};

    GetAllAvailableSeatsForMovieAndTheater query{
        TestData::MovieName,
        TestData::TheaterName,
        seats};

    expect(
        query.Execute(dbInstance) == DatabaseError::OK,
        "Failed to get available seats");


    for (auto seat : TestData::AllSeats)
    {
        auto it = std::find(seats.begin(), seats.end(), seat);
        expect(it != seats.end(), std::format("Seat {} not found", seat));
    }
}

void db_bookaSeat()
{
    MovieSessionDatabase dbInstance;
    auto movie = MovieScreening::create(
        std::string{TestData::MovieName},
        std::string{TestData::TheaterName});

    AddMovie operation(movie);
    expect(operation.Execute(dbInstance) == DatabaseError::OK, "Failed to add movie!");
    
    BookSeatForMovieAndTheater bookRequeat(TestData::MovieName, TestData::TheaterName, TestData::RequestedSeats);

    expect(bookRequeat.Execute(dbInstance) == DatabaseError::OK, "Failed to book a seat");

    BookSeatForMovieAndTheater secondBookRequest(TestData::MovieName, TestData::TheaterName, TestData::RequestedSeats);
    expect(secondBookRequest.Execute(dbInstance) == DatabaseError::SeatUnavailable, "Seat should be unavailable at this stage. Already occupied by previous step");

    std::array<std::string, 20> seats{};

    GetAllAvailableSeatsForMovieAndTheater query{
        TestData::MovieName,
        TestData::TheaterName,
        seats};

    expect(query.Execute(dbInstance) == DatabaseError::OK, "Failed to get available seats");

    for (auto seat : TestData::RequestedSeats)
    {
        auto it = std::find(seats.begin(), seats.end(), seat);
        expect(it == seats.end(), std::format("The list of available seats shall not contain {}", seat));
    }
}

void db_bookingAtomicity()
{
    MovieSessionDatabase dbInstance;
    auto movie = MovieScreening::create(
        std::string{TestData::MovieName},
        std::string{TestData::TheaterName});

    AddMovie operation(movie);
    expect(operation.Execute(dbInstance) == DatabaseError::OK, "Failed to add movie!");

    // First book "A1"
    const auto firstSeatOnly = std::to_array<std::string_view>({"A1"});
    BookSeatForMovieAndTheater firstBooking(TestData::MovieName, TestData::TheaterName, firstSeatOnly);
    expect(firstBooking.Execute(dbInstance) == DatabaseError::OK, "Failed to book A1");

    // Try booking {"A1", "A2"}. Since "A1" is already booked, this entire request must fail.
    const auto attemptBoth = std::to_array<std::string_view>({"A1", "A2"});
    BookSeatForMovieAndTheater secondBooking(TestData::MovieName, TestData::TheaterName, attemptBoth);
    expect(secondBooking.Execute(dbInstance) == DatabaseError::SeatUnavailable, "Booking should fail because A1 is already booked");

    // Now query available seats to ensure "A2" is STILL available (not partially booked)
    std::array<std::string, 20> seats{};
    GetAllAvailableSeatsForMovieAndTheater query{
        TestData::MovieName,
        TestData::TheaterName,
        seats};
    expect(query.Execute(dbInstance) == DatabaseError::OK, "Failed to get available seats");

    auto it = std::find(seats.begin(), seats.end(), "A2");
    expect(it != seats.end(), "A2 should still be available (rollback verification)");
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

    run("Test Visitor memory protection", db_memoryProtectionWhenAddAndGet, failures);
    run("Test MovieScreening object builder", movieScreeningBuilder, failures);
    run("Simple add and get operations", db_addAndGetMovie, failures);
    run("Getting all theaters showing the movie", db_GetAllTheaterShowingTheMovie, failures);
    run("Getting all available seats for the movie", db_getAllAvailableSeats, failures);
    run("Book a seat", db_bookaSeat, failures);
    run("Atomic Booking Transaction", db_bookingAtomicity, failures);
    return failures == 0 ? 0 : 1;
}