#include "config.h"

WARNINGS_OFF
#include <cstdlib>
#include <exception>
#include <iostream>
WARNINGS_ON

#include "device/list_devices.h"
#include "run_application.h"
#include "ui/run_ui.h"

namespace frank::video {

NO_RETURN
void run_application(int, char const *[], void *mock_data) {
  std::set_terminate([]() noexcept {
    std::cerr << "Unhandled exception\n";
    std::abort();
  });
  auto input_devices = list_input_devices(nullptr, mock_data);
  if (input_devices.size() < 1)
    std::cerr << "Could not list video / audio input devices\n";

  run_ui(input_devices, nullptr, mock_data);
}

} // namespace frank::video
