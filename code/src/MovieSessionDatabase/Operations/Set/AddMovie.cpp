#include "AddMovie.hpp"
#include <span>

AddMovie::AddMovie(MovieScreeningType movie): m_data(std::move(movie))
{
}

[[nodiscard]]
DatabaseError AddMovie::visit(std::span<MovieScreeningType> movieslist)
{
    for (auto& movie : movieslist)
    {
        if (!movie)
        {
            movie = std::move(m_data);
            return DatabaseError::OK;
        }
    }

    return DatabaseError::GenericError;
}