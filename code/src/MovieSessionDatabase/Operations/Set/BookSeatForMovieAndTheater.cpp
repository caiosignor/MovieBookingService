#include "BookSeatForMovieAndTheater.hpp"
#include <span>

BookSeatForMovieAndTheater::BookSeatForMovieAndTheater(std::string_view movie,std::string_view theater, std::span<std::string_view> requestedSeats):
    m_movie(movie), m_theater(theater), m_requestedSeat(requestedSeats)
{
    m_isFinished = false;
    m_retCode = DatabaseError::OK;
}

void BookSeatForMovieAndTheater::visit(MovieScreeningPtrType movie)
{
    if(movie->movie_name == m_movie && movie->theater_name == m_theater)
    {
        for(const auto req : m_requestedSeat)
        {
            auto seat = movie->m_availableSeats.find(std::string{req});
            if(seat->second)
            {
                seat->second = false;
            }
            else
            {
                finish();
                m_retCode = DatabaseError::SeatUnavailable;
                return;
            }
        }
        m_retCode = DatabaseError::OK;
        finish();
    }
}