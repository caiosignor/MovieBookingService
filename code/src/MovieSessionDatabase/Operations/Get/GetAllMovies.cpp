#include "GetAllMovies.hpp"

GetAllMovies::GetAllMovies(std::span<MovieScreeningType> out)
    : m_data(out)
{
    m_isFinished = false;
}

void GetAllMovies::visit(MovieScreeningType movie)
{
    if (m_dataIterator >= m_data.size())
    {
        finish();
        m_retCode = DatabaseError::OK;
        return;
    }

    m_data[m_dataIterator++] = movie;
}