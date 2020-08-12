#include "config.h"

WARNINGS_OFF
#include <exception>
#include <iostream>
WARNINGS_ON

#include "device/list_devices.h"
#include "exception/handler.h"
#include "run_application.h"
#include "ui/run_ui.h"

namespace frank::video {

NO_RETURN
auto application(void *mock_data) {
  auto input_devices = list_devices(mock_data);
  if (input_devices.size() < 1)
    std::cerr << "Could not list video / audio input devices\n";

  run_ui(input_devices, nullptr, mock_data);
}

NO_RETURN
void run_application(int, char const *[], void *mock_data) {
  std::set_terminate(unhandled_exception_handler);
  try {
    application(mock_data);
  } catch (std::exception const &e) {
    all_exceptions_handler(&e, mock_data);
  } catch (...) {
    all_exceptions_handler(nullptr, mock_data);
  }
}

} // namespace frank::video
