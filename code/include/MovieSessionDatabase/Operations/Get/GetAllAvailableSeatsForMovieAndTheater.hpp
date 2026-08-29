#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

class GetAllAvailableSeatsForMovieAndTheater : public MovieSessionVisitor
{
public:
    explicit GetAllAvailableSeatsForMovieAndTheater(std::string_view movie,std::string_view theater, std::span<std::string> out);
    ~GetAllAvailableSeatsForMovieAndTheater() override = default;

private:
    std::span<std::string> m_outData;
    std::string_view m_movieName;
    std::string_view m_theaterName;
    size_t m_dataIterator{0};
    void visit(MovieScreeningPtrType movie) override;
};
