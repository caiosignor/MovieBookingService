#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

class GetAllMovies : public MovieSessionVisitor
{
public:
    explicit GetAllMovies(std::span<MovieScreeningType> out);
    ~GetAllMovies() override = default;

    [[nodiscard]]
    DatabaseError Execute();

private:
    std::span<MovieScreeningType> m_data;

    [[nodiscard]]
    DatabaseError visit(std::span<MovieScreeningType> movieslist) override;
};
