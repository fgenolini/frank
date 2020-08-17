#include "config.h"

WARNINGS_OFF
#include <exception>
#include <iostream>
WARNINGS_ON

#include "device/list_devices.h"
#include "exception/exceptions_handler.h"
#include "application.h"
#include "ui/run_ui.h"

namespace frank::video {

application::application(void *mock_data) : mock_data_(mock_data) {}

NO_RETURN
void application::webcam_viewer() noexcept(false) {
  auto input_devices = list_devices(mock_data_);
  if (input_devices.size() < 1)
    std::cerr << "Could not list video / audio input devices\n";
  cvui_init initialise_windows{};
  run_ui(input_devices, initialise_windows, nullptr, mock_data_);
}

NO_RETURN
void application::run(int, char const *[]) {
  std::set_terminate(unhandled_exception_handler);
  try {
    webcam_viewer();
  } catch (std::exception const &e) {
    exceptions_handler(&e, mock_data_);
  } catch (...) {
    exceptions_handler(nullptr, mock_data_);
  }
}

} // namespace frank::video
