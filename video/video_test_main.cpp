#include <cstdlib>
#include <cstring>
#include <iostream>

#include "run_tests.h"
#include "videoConfig.h"

int main(int argc, char const *argv[]) {
  constexpr auto TEST_ARGUMENT = "--test";

  if (argc < 2 || strcmp(argv[1], TEST_ARGUMENT) != 0) {
    std::cerr << argv[0] << " v" << VIDEO_VERSION_MAJOR << "."
              << VIDEO_VERSION_MINOR << '\n';
    std::cerr << "Usage: " << argv[0] << " --test test_name\n";
    exit(EXIT_FAILURE);
  }

  frank::video::run_tests(argc, argv);
}
