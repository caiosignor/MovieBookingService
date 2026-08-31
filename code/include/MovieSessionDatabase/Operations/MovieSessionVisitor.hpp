#pragma once

#include <span>

#include "MovieSessionDatabase.hpp"

/**
 * @brief Result codes returned by database visitors.
 */
enum class DatabaseError { OK, NotFound, OutOfMemory, SeatUnavailable, GenericError };

/**
 * @brief Base class for operations that traverse the movie database.
 *
 * Visitors walk the screening arrays, collect data and update the result
 * code while allowing early termination when needed.
 */
class MovieSessionVisitor
{
public:
    virtual ~MovieSessionVisitor() = default;

    /**
     * @brief Executes the visitor against the default-static database instance.
     * @return Status of the operation.
     */
    [[nodiscard]]
    DatabaseError Execute();

    /**
     * @brief Executes the visitor against a specific database instance.
     * @param instance Database to inspect.
     * @return Status of the operation.
     */
    [[nodiscard]]
    DatabaseError Execute(MovieSessionDatabase& instance);

    /**
     * @brief Called with the screening list before individual records.
     * @param movieslist Collection of screenings.
     */
    virtual void visit(std::span<MovieScreeningPtrType> movieslist){(void)movieslist;}

    /**
     * @brief Called for each screening item.
     * @param movie Screening entry currently being processed.
     */
    virtual void visit(MovieScreeningPtrType movie){(void)movie;}

    /**
     * @brief Checks whether the visitor has requested early termination.
     * @return True when the traversal should stop.
     */
    bool isFinished() const { return m_isFinished; }

    /**
     * @brief Marks the visitor as finished.
     */
    void finish() { m_isFinished = true; }

protected:
    bool m_isFinished{false}; ///< Stops traversal when a visitor completes.
    DatabaseError m_retCode{DatabaseError::OK}; ///< Result of the current operation.
};