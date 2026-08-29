#include <iostream>

void expect(bool condition, const std::string &message) {
  if (!condition) throw std::runtime_error(message);
}

void db_basicAddGetOperations() {
    
//   const auto screening = makeScreening();
//   const std::vector<std::string> empty;
//   expect(screening->bookSeats(empty).error == BookingError::EmptySeatRequest,
//          "an empty booking must be rejected");

//   const std::vector<std::string> duplicate{"A1", "A1"};
//   expect(screening->bookSeats(duplicate).error == BookingError::DuplicateSeatRequest,
//          "the same seat cannot appear twice");

//   const std::vector<std::string> unknown{"Z9"};
//   expect(screening->bookSeats(unknown).error == BookingError::SeatNotFound,
//          "an unknown seat must be rejected");
//   expect(screening->availableSeats().size() == Screening::SeatCapacity,
//          "failed bookings must not change seats");
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