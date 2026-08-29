#include "GetAllMovies.hpp"

GetAllMovies::GetAllMovies(std::span<MovieScreeningPtrType> out)
    : m_data(out)
{
    m_isFinished = false;
    m_retCode = DatabaseError::OK;
}

void GetAllMovies::visit(MovieScreeningPtrType movie)
{
    if (m_dataIterator >= m_data.size())
    {
        finish();
        m_retCode = DatabaseError::OK;
        return;
    }

    m_data[m_dataIterator++] = movie;
    m_retCode = DatabaseError::OK;
}