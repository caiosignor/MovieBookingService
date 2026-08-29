#include "AddMovie.hpp"
#include <span>

AddMovie::AddMovie(MovieScreeningPtrType movie): m_data(std::move(movie))
{
}

void AddMovie::visit(std::span<MovieScreeningPtrType> movieslist)
{
    for (auto& movie : movieslist)
    {
        if (!movie)
        {
            movie = std::move(m_data);
            
            m_retCode = DatabaseError::OK;
        }
    }
}