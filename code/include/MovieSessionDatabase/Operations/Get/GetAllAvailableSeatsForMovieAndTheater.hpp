#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

/**
 * @brief Lists all available seats for a movie in a theater.
 */
class GetAllAvailableSeatsForMovieAndTheater : public MovieSessionVisitor
{
public:
    /**
     * @brief Creates the visitor for a movie, theater and output buffer.
     * @param movie Movie title.
     * @param theater Theater name.
     * @param out Buffer where free seat ids will be written.
     */
    explicit GetAllAvailableSeatsForMovieAndTheater(std::string_view movie,std::string_view theater, std::span<std::string> out);
    ~GetAllAvailableSeatsForMovieAndTheater() override = default;

private:
    std::span<std::string> m_outData; ///< Buffer with free seat names.
    std::string_view m_movieName;     ///< Movie title filter.
    std::string_view m_theaterName;   ///< Theater filter.
    size_t m_dataIterator{0};         ///< Current output index.
    void visit(MovieScreeningPtrType movie) override;
};
