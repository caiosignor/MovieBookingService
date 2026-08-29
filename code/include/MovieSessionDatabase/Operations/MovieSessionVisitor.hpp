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

    virtual void visit(std::span<MovieScreeningType> movieslist){(void)movieslist;}
    virtual void visit(MovieScreeningType movie){(void)movie;}

    bool isFinished() const { return m_isFinished; }
    void finish() { m_isFinished = true; }

protected:
    bool m_isFinished{true};
    DatabaseError m_retCode{DatabaseError::OK};
};