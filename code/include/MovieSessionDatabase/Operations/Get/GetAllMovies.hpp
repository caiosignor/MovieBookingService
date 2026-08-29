#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

class GetAllMovies : public MovieSessionVisitor
{
public:
    explicit GetAllMovies(std::span<MovieScreening> out);
    ~GetAllMovies() override = default;

private:
    std::span<MovieScreening> m_data;
    size_t m_dataIterator{0};
    void visit(MovieScreeningPtrType movie) override;
};
