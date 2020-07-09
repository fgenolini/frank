#include <cstdlib>
#include <cstring>
#include <iostream>

#include "list_devices.h"
#include "opencv_ui.h"
#include "run_tests.h"
#include "videoConfig.h"

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cout << argv[0] << " v" << VIDEO_VERSION_MAJOR << "."
              << VIDEO_VERSION_MINOR << "\n";
    std::cout << "Usage: " << argv[0] << " number"
              << "\n";
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "test") == 0) {
    frank::video::run_tests(argc, argv);
  }

  auto input_devices = frank::video::list_input_devices();
  if (input_devices.size() < 1) {
    std::cerr << "Could not list video / audio input devices"
              << "\n";
  }

  frank::video::opencv_ui(input_devices);
}
