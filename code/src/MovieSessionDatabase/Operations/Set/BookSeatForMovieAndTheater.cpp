#include "BookSeatForMovieAndTheater.hpp"
#include <span>

BookSeatForMovieAndTheater::BookSeatForMovieAndTheater(const std::string_view movie, const std::string_view theater, std::span<const std::string_view> requestedSeats):
    m_movie(movie), m_theater(theater), m_requestedSeat(requestedSeats)
{
    m_isFinished = false;
    m_retCode = DatabaseError::OK;
}

void BookSeatForMovieAndTheater::visit(MovieScreeningPtrType movie)
{
    if(movie->movie_name.compare(m_movie) == 0 &&
        movie->theater_name.compare(m_theater)==0)
    {
        for(const auto req : m_requestedSeat)
        {
            auto seat = movie->m_availableSeats.find(std::string{req});
            if(seat != movie->m_availableSeats.end() && seat->second)
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