#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

class MovieSessionVisitor;
enum class DatabaseError;

struct _MovieScreening
{
    std::string movie_id{};
    std::string theater_id{};
    std::string movie_name{};
    std::string theater_name{};

    _MovieScreening() = default;

    _MovieScreening(
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

    static std::shared_ptr<_MovieScreening> create(
        std::string movieId,
        std::string theaterId,
        std::string movieName,
        std::string theaterName)
    {
        return std::make_shared<_MovieScreening>(
            std::move(movieId),
            std::move(theaterId),
            std::move(movieName),
            std::move(theaterName));
    }
};

using MovieScreeningType = std::shared_ptr<_MovieScreening>;

class MovieSessionDatabase
{
    friend class MovieSessionVisitor;
public:
    DatabaseError accept(MovieSessionVisitor& visitor);
    MovieSessionDatabase();

private:
    static constexpr std::size_t DATABASE_CAPACITY = 20;
    std::vector<MovieScreeningType> m_screeningMovies;
};