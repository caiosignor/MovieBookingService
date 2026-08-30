#pragma once

#include <array>
#include <string_view>
#include <array>
#include <string_view>

namespace TestData
{
    inline constexpr std::string_view MovieName = "The Dark Knight";
    inline constexpr std::string_view TheaterName = "Cineplex";

    inline constexpr auto Movies = std::to_array({
        std::pair{std::string_view{"The Dark Knight"}, std::string_view{"Cineplex"}},
        std::pair{std::string_view{"The Dark Knight"}, std::string_view{"IMAX"}},
        std::pair{std::string_view{"Inception"}, std::string_view{"IMAX"}},
        std::pair{std::string_view{"Interstellar"}, std::string_view{"Cinemark"}},
        std::pair{std::string_view{"The Matrix"}, std::string_view{"AMC"}},
        std::pair{std::string_view{"Pulp Fiction"}, std::string_view{"Regal"}},
        std::pair{std::string_view{"Fight Club"}, std::string_view{"Odeon"}},
        std::pair{std::string_view{"The Godfather"}, std::string_view{"Cineworld"}},
        std::pair{std::string_view{"Forrest Gump"}, std::string_view{"Cinemax"}},
        std::pair{std::string_view{"The Shawshank Redemption"}, std::string_view{"Cineplex"}},
        std::pair{std::string_view{"Gladiator"}, std::string_view{"IMAX"}},
        std::pair{std::string_view{"The Lord of the Rings"}, std::string_view{"Cinemark"}},
        std::pair{std::string_view{"Saving Private Ryan"}, std::string_view{"Regal"}},
        std::pair{std::string_view{"The Departed"}, std::string_view{"Odeon"}},
        std::pair{std::string_view{"Whiplash"}, std::string_view{"Cineworld"}},
        std::pair{std::string_view{"Parasite"}, std::string_view{"Cinemax"}},
        std::pair{std::string_view{"Goodfellas"}, std::string_view{"Cineplex"}},
        std::pair{std::string_view{"The Prestige"}, std::string_view{"IMAX"}},
        std::pair{std::string_view{"Django Unchained"}, std::string_view{"Cinemark"}},
        std::pair{std::string_view{"Mad Max: Fury Road"}, std::string_view{"AMC"}}

    });

    inline constexpr auto RequestedSeats = std::to_array<std::string_view>({
        "A1",
        "A2",
        "A3"
    });

    inline constexpr auto AllSeats = std::to_array<std::string_view>({
        "A1", "A2", "A3", "A4", "A5",
        "B1", "B2", "B3", "B4", "B5",
        "C1", "C2", "C3", "C4", "C5",
        "D1", "D2", "D3", "D4", "D5"
    });
}