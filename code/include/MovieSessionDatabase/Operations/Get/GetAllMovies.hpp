#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

/**
 * @brief Collects the list of movie titles currently available in the database.
 */
class GetAllMovies : public MovieSessionVisitor
{
public:
    /**
     * @brief Creates the visitor with an output buffer.
     * @param out Buffer where movie names will be written.
     */
    explicit GetAllMovies(std::span<MovieScreening> out);
    ~GetAllMovies() override = default;

private:
    std::span<MovieScreening> m_data; ///< Buffer used to write results.
    size_t m_dataIterator{0};         ///< Current output index.
    void visit(MovieScreeningPtrType movie) override;
};
