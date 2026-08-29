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
        std::pair{std::string_view{"Inception"}, std::string_view{"IMAX"}},
        std::pair{std::string_view{"Interstellar"}, std::string_view{"Cinemark"}}
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