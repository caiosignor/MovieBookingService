#include "MovieSessionDatabase/MovieSessionDatabase.hpp"
#include "MovieSessionVisitor.hpp"

MovieSessionDatabase::MovieSessionDatabase()
    : m_screeningMovies(DATABASE_CAPACITY, nullptr)
{
}

void MovieSessionDatabase::accept(MovieSessionVisitor& visitor)
{
    std::lock_guard lock(m_mutex);
    visitor.visit(m_screeningMovies);

    for (const auto& movie : m_screeningMovies)
    {
        if (visitor.isFinished())
        {
            break;
        }

        if (movie)
        {
            visitor.visit(movie);
        }
        else
        {
            visitor.finish();
        }
    }
}