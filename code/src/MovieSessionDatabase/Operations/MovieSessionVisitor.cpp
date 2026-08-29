#include "MovieSessionVisitor.hpp"

[[nodiscard]]
DatabaseError MovieSessionVisitor::Execute()
{
    static MovieSessionDatabase database;
    return Execute(database);
}

[[nodiscard]]
DatabaseError MovieSessionVisitor::Execute(MovieSessionDatabase& instance)
{
    instance.accept(*this);
    return m_retCode;
}
