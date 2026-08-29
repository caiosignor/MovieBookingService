#pragma once

#include <span>

#include "MovieSessionDatabase.hpp"

enum class DatabaseError { OK, NotFound, GenericError };

class MovieSessionVisitor
{
public:
    virtual ~MovieSessionVisitor() = default;

    [[nodiscard]]
    DatabaseError Execute();

    [[nodiscard]]
    virtual DatabaseError visit(std::span<MovieScreeningType> movieslist) = 0;
};