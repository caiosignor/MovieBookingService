#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

class MovieSessionVisitor;
enum class DatabaseError;

class MovieScreening
{
public:
    static std::shared_ptr<MovieScreening> create(
        std::string movieId,
        std::string theaterId,
        std::string movieName,
        std::string theaterName)
    {
        return std::make_shared<MovieScreening>(
            std::move(movieId),
            std::move(theaterId),
            std::move(movieName),
            std::move(theaterName));
    }

    MovieScreening() = default;

    MovieScreening(
        std::string movieId,
        std::string theaterId,
        std::string movieName,
        std::string theaterName)
        : movie_id(std::move(movieId))
        , theater_id(std::move(theaterId))
        , movie_name(std::move(movieName))
        , theater_name(std::move(theaterName))
    {
    }

    std::string movie_id{};
    std::string theater_id{};
    std::string movie_name{};
    std::string theater_name{};
};

using MovieScreeningPtrType = std::shared_ptr<MovieScreening>;

class MovieSessionDatabase
{
    friend class MovieSessionVisitor;
public:
    void accept(MovieSessionVisitor& visitor);
    MovieSessionDatabase();

private:
    static constexpr std::size_t DATABASE_CAPACITY = 20;
    std::vector<MovieScreeningPtrType> m_screeningMovies;
};