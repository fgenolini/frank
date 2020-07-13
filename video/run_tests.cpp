#include <cstdlib>
#include <exception>
#include <iostream>

#include "run_tests.h"

namespace frank::video {

[[noreturn]] void run_tests(int argc, char const *argv[]) {
  std::set_terminate([]() {
    std::cerr << "Unhandled exception\n";
    std::abort();
  });
  if (argc < 3) {
    exit(EXIT_SUCCESS);
  }

  // TODO: write an actual test
  exit(EXIT_FAILURE);
}

} // namespace frank::video
