#include "GetAllMovies.hpp"

GetAllMovies::GetAllMovies(std::span<MovieScreeningType> out)
    : m_data(out)
{
}

[[nodiscard]]
DatabaseError GetAllMovies::Execute()
{
    for (auto& slot : m_data)
    {
        slot = nullptr;
    }

    if (!m_data.empty())
    {
        m_data[0] = std::make_unique<_MovieScreening>(
            "movie-id-1",
            "movie-id-1",
            "movie-id-1",
            "movie-id-1");
    }

    return DatabaseError::OK;
}

[[nodiscard]]
DatabaseError GetAllMovies::visit(std::span<MovieScreeningType> movieslist)
{
    (void)movieslist;
    return DatabaseError::OK;
}