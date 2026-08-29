#include "AddMovie.hpp"
#include <span>

AddMovie::AddMovie(MovieScreeningType movie): m_data(std::move(movie))
{

}

[[nodiscard]]
DatabaseError AddMovie::visit(std::span<MovieScreeningType> movieslist)
{
    if(movieslist.capacity())
    {
        moviesList.insert(m_data);
    }else
    {
        return DatabaseError::GenericError;
    }
}