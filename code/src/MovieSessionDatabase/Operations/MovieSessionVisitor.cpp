#include "MovieSessionDatabase/MovieSessionVisitor.hpp"

[[nodiscard]]
DatabaseError MovieSessionVisitor::visit(std::span<MovieScreeningType> movieslist)
{
    static MovieSessionDatabase database;
    database.accept(*this);

    return DatabaseError::OK;
}
