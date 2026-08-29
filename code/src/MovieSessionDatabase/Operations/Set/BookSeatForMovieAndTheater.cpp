#include "BookSeatForMovieAndTheater.hpp"
#include <span>

BookSeatForMovieAndTheater::BookSeatForMovieAndTheater(const std::string_view movie, const std::string_view theater, std::span<const std::string_view> requestedSeats):
    m_movie(movie), m_theater(theater), m_requestedSeat(requestedSeats)
{
    m_retCode = DatabaseError::OK;
}

void BookSeatForMovieAndTheater::visit(MovieScreeningPtrType movie)
{
    if (movie->movie_name.compare(m_movie) == 0 &&
        movie->theater_name.compare(m_theater) == 0)
    {
        // First pass: Verify all requested seats are available
        for (const auto req : m_requestedSeat)
        {
            auto seat = movie->m_availableSeats.find(std::string{req});
            if (seat == movie->m_availableSeats.end() || !seat->second)
            {
                m_retCode = DatabaseError::SeatUnavailable;
                finish();
                return;
            }
        }

        // Second pass: Book all requested seats
        for (const auto req : m_requestedSeat)
        {
            movie->m_availableSeats[std::string{req}] = false;
        }

        m_retCode = DatabaseError::OK;
        finish();
    }
}