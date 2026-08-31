#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

/**
 * @brief Inserts a new movie screening into the database.
 */
class AddMovie : public MovieSessionVisitor
{
public:
    /**
     * @brief Creates the insertion operation.
     * @param movie Screening to add.
     */
    explicit AddMovie(MovieScreeningPtrType movie);
    ~AddMovie() override = default;

private:
    MovieScreeningPtrType m_data; ///< Screening being inserted.

    void visit(std::span<MovieScreeningPtrType> movieslist) override;
};
