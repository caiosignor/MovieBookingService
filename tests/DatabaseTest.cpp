#include <iostream>
#include "AddMovie.hpp"

void expect(bool condition, const std::string &message) {
  if (!condition) throw std::runtime_error(message);
}

void db_basicAddGetOperations() {

    MovieScreeningType movie = std::make_unique<struct _MovieScreening>(
        "movie-id-1",
        "movie-id-1",
        "movie-id-1",
        "movie-id-1"
    );

    expect(AddMovie(std::move(movie)).Execute() == DatabaseError::OK, "Fail to add data to database");
    

}

void run(const char *name, void (*test)(), int &failures) {
  try {
    test();
    std::cout << "[PASS] " << name << '\n';
  } catch (const std::exception &error) {
    ++failures;
    std::cerr << "[FAIL] " << name << ": " << error.what() << '\n';
  }
}


int main() {
  int failures = 0;
  run("invalid bookings", db_basicAddGetOperations, failures);
  return failures == 0 ? 0 : 1;
}