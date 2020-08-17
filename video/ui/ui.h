#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "device/input_device.h"
#include "test/testable_exit.h"
#include "ui/user_interface.h"

namespace frank::video {

class ui {
public:
  ui(std::vector<input_device> const &connected_webcams,
     cvui_init *initialise_windows = nullptr,
     user_interface_factory *make_ui = nullptr,
     exiter *injected_exiter = nullptr);

  virtual ~ui();

  NO_RETURN virtual void run();

private:
  void interface_with_user();

  cvui_init *initialise_windows_;
  exiter *exiter_;
  std::vector<input_device> const &connected_webcams_;
  user_interface_factory *make_ui_;
};

} // namespace frank::video
