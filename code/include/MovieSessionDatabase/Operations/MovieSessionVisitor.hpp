#pragma once

#include <span>

#include "MovieSessionDatabase.hpp"

enum class DatabaseError { OK, NotFound, GenericError };

class MovieSessionVisitor
{
public:
    [[nodiscard]]
    DatabaseError Execute();

private:
    [[nodiscard]]
    virtual DatabaseError visit(std::span<MovieScreeningType> movieslist) = 0;
};