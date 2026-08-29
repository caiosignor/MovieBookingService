#include "GetAllTheaterShowingTheMovie.hpp"

GetAllTheaterShowingTheMovie::GetAllTheaterShowingTheMovie(std::string_view movie, std::span<std::string> out)
    : m_outData(out), m_movieName(movie)
{
    m_isFinished = false;
    m_retCode = DatabaseError::NotFound;
}

void GetAllTheaterShowingTheMovie::visit(MovieScreeningPtrType movie)
{
    if (m_dataIterator >= m_outData.size())
    {
        finish();
        m_retCode = DatabaseError::OutOfMemory;
        return;
    }

    if(movie->movie_name.compare(m_movieName) == 0)
    {
        m_outData[m_dataIterator++] = movie->theater_name;
        m_retCode = DatabaseError::OK;
    }

}