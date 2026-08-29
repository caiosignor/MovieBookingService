#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

class AddMovie : public MovieSessionVisitor
{
public:
    explicit AddMovie(MovieScreeningType movie);
    ~AddMovie() override = default;

private:
    MovieScreeningType m_data;

    void visit(std::span<MovieScreeningType> movieslist) override;
};
