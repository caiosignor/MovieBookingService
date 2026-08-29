#include "AddMovie.hpp"
#include <span>

AddMovie::AddMovie(MovieScreeningType movie): m_data(std::move(movie))
{
}

void AddMovie::visit(std::span<MovieScreeningType> movieslist)
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