#include <cstdlib>
#include <exception>
#include <iostream>

#include "list_devices.h"
#include "opencv_ui.h"
#include "run_application.h"

namespace frank::video {

[[noreturn]] void run_application(int argc, char const *argv[]) {
  std::set_terminate([]() {
    std::cout << "Unhandled exception\n";
    std::abort();
  });
  auto input_devices = list_input_devices();
  if (input_devices.size() < 1) {
    std::cerr << "Could not list video / audio input devices"
              << "\n";
  }

  opencv_ui(input_devices);
}

} // namespace frank::video
