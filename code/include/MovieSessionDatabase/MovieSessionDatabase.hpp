#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <unordered_map>

class MovieSessionVisitor;
enum class DatabaseError;

/**
 * @brief Represents a movie screening in a theater.
 *
 * Each screening stores the movie name, the theater name and the seat map.
 * The seat map is initialized as available for all default seats.
 */
class MovieScreening
{
public:
    /**
     * @brief Factory helper to create a screening instance.
     * @param movieName Movie title.
     * @param theaterName Theater name.
     * @return Shared pointer to the new screening.
     */
    static std::shared_ptr<MovieScreening> create(
        std::string movieName,
        std::string theaterName)
    {
        return std::make_shared<MovieScreening>(
            std::move(movieName),
            std::move(theaterName));
    }

    MovieScreening() = default;

    /**
     * @brief Constructs a screening with the given movie and theater.
     * @param movieName Movie title.
     * @param theaterName Theater name.
     */
    MovieScreening(
        std::string movieName,
        std::string theaterName)
        : movie_name(std::move(movieName)), theater_name(std::move(theaterName))
    {
    }

    std::string movie_name{}; ///< Movie title.
    std::string theater_name{}; ///< Theater that shows it.

    // true means it is available, false otherwise
    std::unordered_map<std::string, bool> m_availableSeats{
        {"A1", true},
        {"A2", true},
        {"A3", true},
        {"A4", true},
        {"A5", true},
        {"B1", true},
        {"B2", true},
        {"B3", true},
        {"B4", true},
        {"B5", true},
        {"C1", true},
        {"C2", true},
        {"C3", true},
        {"C4", true},
        {"C5", true},
        {"D1", true},
        {"D2", true},
        {"D3", true},
        {"D4", true},
        {"D5", true}
    }; ///< Seat availability map.
};

using MovieScreeningPtrType = std::shared_ptr<MovieScreening>;

/**
 * @brief In-memory database of movie screenings.
 *
 * The database stores screenings in a fixed-size list and exposes a visitor
 * interface to query or mutate entries in a controlled way.
 */
class MovieSessionDatabase
{
    friend class MovieSessionVisitor;

public:
    /**
     * @brief Starts a visitor traversal over the stored screenings.
     * @param visitor Visitor to apply.
     */
    void accept(MovieSessionVisitor &visitor);

    MovieSessionDatabase();

    /**
     * @brief Returns how many screening slots are currently allocated.
     * @return Total stored screenings.
     */
    size_t getScreeningCount() const {
        return m_screeningMovies.size();
    }

private:
    static constexpr std::size_t DATABASE_CAPACITY = 20; ///< Maximum number of screenings supported.
    std::vector<MovieScreeningPtrType> m_screeningMovies; ///< Screening entries.
    std::mutex m_mutex; ///< Protects concurrent access to the database.
};