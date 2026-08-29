#include "MovieSessionVisitor.hpp"

[[nodiscard]]
DatabaseError MovieSessionVisitor::Execute()
{
    static MovieSessionDatabase database;
    database.accept(*this);
    return m_retCode;
}
