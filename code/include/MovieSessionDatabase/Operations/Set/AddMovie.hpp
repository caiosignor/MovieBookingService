#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

class AddMovie : public MovieSessionVisitor
{
public:
    explicit AddMovie(MovieScreeningPtrType movie);
    ~AddMovie() override = default;

private:
    MovieScreeningPtrType m_data;

    void visit(std::span<MovieScreeningPtrType> movieslist) override;
};
