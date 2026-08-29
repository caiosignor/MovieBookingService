#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

class GetAllMovies : public MovieSessionVisitor
{
public:
    explicit GetAllMovies(std::span<MovieScreeningPtrType> out);
    ~GetAllMovies() override = default;

private:
    std::span<MovieScreeningPtrType> m_data;
    size_t m_dataIterator{0};
    void visit(MovieScreeningPtrType movie) override;
};
