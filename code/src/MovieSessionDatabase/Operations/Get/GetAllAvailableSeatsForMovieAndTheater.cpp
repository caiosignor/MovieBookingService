#include "GetAllAvailableSeatsForMovieAndTheater.hpp"

GetAllAvailableSeatsForMovieAndTheater::GetAllAvailableSeatsForMovieAndTheater(std::string_view movie,std::string_view theater, std::span<std::string> out)
    : m_outData(out), m_movieName(movie), m_theaterName(theater)
{
    m_retCode = DatabaseError::NotFound;
}

void GetAllAvailableSeatsForMovieAndTheater::visit(MovieScreeningPtrType movie)
{
    if (m_dataIterator >= m_outData.size())
    {
        finish();
        m_retCode = DatabaseError::OutOfMemory;
        return;
    }

    if(movie->movie_name.compare(m_movieName) == 0 && movie->theater_name.compare(m_theaterName) ==0)
    {
        for(const auto& [seat, isfree] : movie->m_availableSeats)
        {
            if(isfree)
            {
                if (m_dataIterator >= m_outData.size())
                {
                    finish();
                    m_retCode = DatabaseError::OutOfMemory;
                    return;
                }

                m_outData[m_dataIterator++] = seat;
            }
        }
        finish();
        m_retCode = DatabaseError::OK;
    }
}