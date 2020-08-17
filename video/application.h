#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "device/input_device.h"
#include "device/video_devices.h"
#include "exception/exceptions.h"
#include "test/testable_exit.h"
#include "ui/ui.h"

namespace frank::video {

class application {
public:
  application(video_devices *devices = nullptr, ui *ui_runner = nullptr,
              exceptions *exception_handler = nullptr);
  virtual ~application();

  NO_RETURN virtual void run(int argc = 0, char const *argv[] = nullptr);

private:
  std::vector<input_device> list_devices();

  NO_RETURN void
  run_ui(std::vector<input_device> const &devices) noexcept(false);

  NO_RETURN void webcam_viewer() noexcept(false);

  exceptions *exception_handler_;
  ui *ui_runner_;
  video_devices *devices_;
};

} // namespace frank::video
