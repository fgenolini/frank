#include "config.h"

WARNINGS_OFF
#include <exception>
#include <iostream>
WARNINGS_ON

#include "application.h"

namespace frank::video {

application::application(video_devices *devices, ui *ui_runner,
                         exceptions *exception_handler)
    : exception_handler_(exception_handler), ui_runner_(ui_runner),
      devices_(devices) {}

std::vector<input_device> application::list_devices() {
  video_devices default_devices{};
  auto devices = devices_ ? devices_ : &default_devices;
  auto input_devices = devices->list();
  if (input_devices.size() < 1)
    std::cerr << "Could not list video / audio input devices\n";

  return input_devices;
}

NO_RETURN void
application::run_ui(std::vector<input_device> const &devices) noexcept(false) {
  cvui_init initialise_windows{};
  ui default_ui_runner(devices, &initialise_windows);
  auto runner = ui_runner_ ? ui_runner_ : &default_ui_runner;
  runner->run();
}

NO_RETURN
void application::webcam_viewer() noexcept(false) {
  auto devices = list_devices();
  run_ui(devices);
}

NO_RETURN
void application::run(int, char const *[]) {
  std::set_terminate(unhandled_exception_handler);
  exceptions default_handler{};
  auto handler = exception_handler_ ? exception_handler_ : &default_handler;
  try {
    webcam_viewer();
  } catch (std::exception const &e) {
    handler->handler(&e);
  } catch (...) {
    handler->handler();
  }
}

} // namespace frank::video
