#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

/**
 * @brief Finds theaters that currently show a selected movie.
 */
class GetAllTheaterShowingTheMovie : public MovieSessionVisitor
{
public:
    /**
     * @brief Creates the visitor for a movie and output buffer.
     * @param movie Movie title to search.
     * @param out Buffer where theater names will be written.
     */
    explicit GetAllTheaterShowingTheMovie(std::string_view movie, std::span<std::string> out);
    ~GetAllTheaterShowingTheMovie() override = default;

private:
    std::span<std::string> m_outData; ///< Result buffer for theater names.
    std::string_view m_movieName;     ///< Movie being searched.
    size_t m_dataIterator{0};         ///< Current output position.
    void visit(MovieScreeningPtrType movie) override;
};
