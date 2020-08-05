#include <cstdlib>
#include <exception>
#include <iostream>

#include "device/list_devices.h"
#include "run_application.h"
#include "ui/opencv_ui.h"

namespace frank::video {

#if !defined(_TEST_MAIN_2_) && !defined(_TEST_WINMAIN_4_) &&                   \
    !defined(_TEST_RUN_APPLICATION_2_)
[[noreturn]]
#endif
 void run_application(int argc, char const *argv[]) {
  std::set_terminate([]() {
    std::cerr << "Unhandled exception\n";
    std::abort();
  });
  auto input_devices = list_input_devices();
  if (input_devices.size() < 1) {
    std::cerr << "Could not list video / audio input devices\n";
  }

  opencv_ui(input_devices);
}

} // namespace frank::video
