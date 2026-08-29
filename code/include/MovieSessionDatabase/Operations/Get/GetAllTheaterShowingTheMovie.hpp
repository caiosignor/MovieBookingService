#pragma once

#include <span>
#include <utility>

#include "MovieSessionVisitor.hpp"

class GetAllTheaterShowingTheMovie : public MovieSessionVisitor
{
public:
    explicit GetAllTheaterShowingTheMovie(std::string_view movie, std::span<std::string> out);
    ~GetAllTheaterShowingTheMovie() override = default;

private:
    std::span<std::string> m_outData;
    std::string_view m_movieName;
    size_t m_dataIterator{0};
    void visit(MovieScreeningPtrType movie) override;
};
