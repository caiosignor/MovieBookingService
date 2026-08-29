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

void MovieSessionDatabase::accept(MovieSessionVisitor& visitor)
{
    visitor.visit(std::span<MovieScreeningType>(m_screeningMovies.data(), m_screeningMovies.size()));

    for(auto movie = m_screeningMovies.begin(); movie != m_screeningMovies.end() && !visitor.isFinished(); ++movie)
    {
        if(*movie)
        {
            visitor.visit(*movie);
        }
        else
        {
            visitor.finish();
        }
    }
}