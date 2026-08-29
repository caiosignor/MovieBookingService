#include "MovieSessionVisitor.hpp"

[[nodiscard]]
DatabaseError MovieSessionVisitor::Execute()
{
    static MovieSessionDatabase database;
    return database.accept(*this);
}
