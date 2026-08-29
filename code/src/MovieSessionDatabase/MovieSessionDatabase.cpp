#include "MovieSessionDatabase/MovieSessionDatabase.hpp"
#include "MovieSessionVisitor.hpp"

MovieSessionDatabase::MovieSessionDatabase()
{
    m_screeningMovies.resize(DATABASE_CAPACITY);
    for (auto& movie : m_screeningMovies)
    {
        movie = nullptr;
    }
}

DatabaseError MovieSessionDatabase::accept(MovieSessionVisitor& visitor)
{
    return visitor.visit(std::span<MovieScreeningType>(m_screeningMovies.data(), m_screeningMovies.size()));
}