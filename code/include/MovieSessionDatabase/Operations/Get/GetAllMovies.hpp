#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

class GetAllMovies : public MovieSessionVisitor
{
public:
    explicit GetAllMovies(std::span<MovieScreeningType> out);
    ~GetAllMovies() override = default;

private:
    std::span<MovieScreeningType> m_data;
    size_t m_dataIterator{0};
    void visit(MovieScreeningType movie) override;
};
