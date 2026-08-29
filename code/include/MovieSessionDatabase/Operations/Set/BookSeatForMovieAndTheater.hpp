#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

class BookSeatForMovieAndTheater : public MovieSessionVisitor
{
public:
    explicit BookSeatForMovieAndTheater(const std::string_view movie, const std::string_view theater, std::span<const std::string_view> requestedSeats);
    ~BookSeatForMovieAndTheater() override = default;

private:
    const std::string_view m_movie, m_theater;
    std::span<const std::string_view> m_requestedSeat;
    void visit(MovieScreeningPtrType movieslist) override;
};
