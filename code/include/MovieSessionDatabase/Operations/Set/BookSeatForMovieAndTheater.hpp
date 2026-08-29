#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

class BookSeatForMovieAndTheater : public MovieSessionVisitor
{
public:
    explicit BookSeatForMovieAndTheater(std::string_view movie,std::string_view theater, std::span<std::string_view> requestedSeats);
    ~BookSeatForMovieAndTheater() override = default;

private:
    std::string_view m_movie, m_theater;
    std::span<std::string_view> m_requestedSeat;
    void visit(MovieScreeningPtrType movieslist) override;
};
