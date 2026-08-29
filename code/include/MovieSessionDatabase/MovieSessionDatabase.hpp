#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <unordered_map>

class MovieSessionVisitor;
enum class DatabaseError;

class MovieScreening
{
public:
    static std::shared_ptr<MovieScreening> create(
        std::string movieName,
        std::string theaterName)
    {
        return std::make_shared<MovieScreening>(
            std::move(movieName),
            std::move(theaterName));
    }

    MovieScreening() = default;

    MovieScreening(
        std::string movieName,
        std::string theaterName)
        : movie_name(std::move(movieName)), theater_name(std::move(theaterName))
    {
    }

    std::string movie_name{};
    std::string theater_name{};
    //true means it is available, false otherwise
    std::unordered_map<std::string, bool> m_availableSeats{
        {"A1", true},
        {"A2", true},
        {"A3", true},
        {"A4", true},
        {"A5", true},
        {"B1", true},
        {"B2", true},
        {"B3", true},
        {"B4", true},
        {"B5", true},
        {"C1", true},
        {"C2", true},
        {"C3", true},
        {"C4", true},
        {"C5", true},
        {"D1", true},
        {"D2", true},
        {"D3", true},
        {"D4", true},
        {"D5", true}
    };
};

using MovieScreeningPtrType = std::shared_ptr<MovieScreening>;

class MovieSessionDatabase
{
    friend class MovieSessionVisitor;

public:
    void accept(MovieSessionVisitor &visitor);
    MovieSessionDatabase();

    size_t getScreeningCount() const {
        return m_screeningMovies.size();
    }

private:
    static constexpr std::size_t DATABASE_CAPACITY = 20;
    std::vector<MovieScreeningPtrType> m_screeningMovies;
};