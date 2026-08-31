#include "BookSeatForMovieAndTheater.hpp"
#include <span>

BookSeatForMovieAndTheater::BookSeatForMovieAndTheater(const std::string_view movie, const std::string_view theater, std::span<const std::string_view> requestedSeats):
    m_movie(movie), m_theater(theater), m_requestedSeat(requestedSeats)
{
    m_retCode = DatabaseError::OK;
}

void BookSeatForMovieAndTheater::visit(MovieScreeningPtrType movie)
{
    // Only modify the record that matches the movie and theater sent by the client.
    if (movie->movie_name.compare(m_movie) == 0 &&
        movie->theater_name.compare(m_theater) == 0)
    {
        // First pass: validate that all requested seats are still available.
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

        // Second pass: mark all requested seats as occupied.
        for (const auto req : m_requestedSeat)
        {
            movie->m_availableSeats[std::string{req}] = false;
        }

        m_retCode = DatabaseError::OK;
        finish();
    }
}