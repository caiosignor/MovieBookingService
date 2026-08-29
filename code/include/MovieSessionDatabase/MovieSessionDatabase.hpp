#pragma once
#include <string>
#include <array>
#include "MovieSessionVisitor.hpp"
#include <memory>

using MovieScreeningType = std::unique_ptr<struct _MovieScreening>
{
    std::string movie_id;
    std::string theater_id;
    std::string movie_name;
    std::string theater_name;
};

class MovieSessionDatabase
{
    friend class MovieSessionVisitor;
public:

    void accept(MovieSessionVisitor& visitor);

    MovieSessionDatabase();
private:
    constexpr static size_t DATABASE_CAPACITY=20;
    std::vector<MovieScreeningType> m_screeningMovies;

};