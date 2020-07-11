#include <cstdlib>
#include <cstring>
#include <iostream>

#include "run_application.h"
#include "run_tests.h"
#include "videoConfig.h"

int main(int argc, char const *argv[]) {
  constexpr auto TEST_ARGUMENT = "--test";

  if (argc < 2) {
    std::cout << argv[0] << " v" << VIDEO_VERSION_MAJOR << "."
              << VIDEO_VERSION_MINOR << "\n";
    std::cout << "Usage: " << argv[0] << " number"
              << "\n";
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], TEST_ARGUMENT) == 0) {
    frank::video::run_tests(argc, argv);
  }

  frank::video::run_application(argc, argv);
}
