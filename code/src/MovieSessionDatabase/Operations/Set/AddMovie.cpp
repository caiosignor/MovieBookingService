#include "AddMovie.hpp"
#include <span>

AddMovie::AddMovie(MovieScreeningPtrType movie): m_data(std::move(movie))
{
}

void AddMovie::visit(std::span<MovieScreeningPtrType> movieslist)
{
    // Find the first free slot to insert the new movie.
    for (auto& movie : movieslist)
    {
        if (!movie)
        {
            movie = std::move(m_data);
            m_retCode = DatabaseError::OK;
            finish();
            return;
        }
    }

    // If there is no space left, the operation fails with an explicit status.
    m_retCode = DatabaseError::OutOfMemory;
    finish();
    return;
}