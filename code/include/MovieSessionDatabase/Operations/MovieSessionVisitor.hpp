#pragma once
#include "MovieSessionDatabase.hpp"

#include <span>
enum class DatabaseError {OK, NotFound, GenericError};

class MovieSessionVisitor
{        
public:
    [[nodiscard]]
    DatabaseError Execute();

private:
    [[nodiscard]]
    virtual DatabaseError visit(std::span<MovieScreeningType> movieslist) = 0;


};