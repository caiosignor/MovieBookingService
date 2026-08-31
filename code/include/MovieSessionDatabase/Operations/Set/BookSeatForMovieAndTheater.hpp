#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

/**
 * @brief Books one or more seats for a movie in a theater.
 */
class BookSeatForMovieAndTheater : public MovieSessionVisitor
{
public:
    /**
     * @brief Creates the booking operation.
     * @param movie Movie title.
     * @param theater Theater name.
     * @param requestedSeats Seats to be booked.
     */
    explicit BookSeatForMovieAndTheater(const std::string_view movie, const std::string_view theater, std::span<const std::string_view> requestedSeats);
    ~BookSeatForMovieAndTheater() override = default;

private:
    const std::string_view m_movie; ///< Movie to book.
    const std::string_view m_theater; ///< Theater to book.
    std::span<const std::string_view> m_requestedSeat; ///< Requested seats.
    void visit(MovieScreeningPtrType movieslist) override;
};
