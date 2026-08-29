#pragma once
#include "MovieSessionVisitor.hpp"

class AddMovie : MovieSessionVisitor {
public:
    AddMovie(MovieScreeningType movie);
private:
    MovieScreeningType m_data;

    [[nodiscard]]
    virtual DatabaseError visit(std::span<MovieScreeningType> movieslist) = 0;
}
