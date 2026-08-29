#include "MovieSessionDatabase/MovieSessionDatabase.hpp"

MovieSessionDatabase::MovieSessionDatabase()
{
    m_screeningMovies.reserve(DATABASE_CAPACITY);
}