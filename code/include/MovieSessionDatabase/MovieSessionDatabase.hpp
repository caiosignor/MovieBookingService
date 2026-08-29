#pragma once

#include <memory>
#include <string>
#include <vector>

class MovieSessionVisitor;

struct _MovieScreening
{
    std::string movie_id;
    std::string theater_id;
    std::string movie_name;
    std::string theater_name;
};

using MovieScreeningType = std::unique_ptr<_MovieScreening>;

class MovieSessionDatabase
{
    friend class MovieSessionVisitor;
public:
    void accept(MovieSessionVisitor& visitor);
    MovieSessionDatabase();

private:
    static constexpr std::size_t DATABASE_CAPACITY = 20;
    std::vector<MovieScreeningType> m_screeningMovies;
};