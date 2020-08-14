#pragma once

#include "config.h"

#include "ui/protected_cvui.h"

#include "device/input_device.h"
#include "ui/application_state.h"
#include "ui/ui_controls.h"

namespace frank::video {

WARNING_PUSH
DISABLE_WARNING_MSC(4820)
class main_settings_window {
public:
  main_settings_window(ui_controls const &controls, application_state &state,
                       void *mock_data = nullptr);

  void draw(bool settings_minimised);

private:
  void draw_canny();
  void draw_webcam(int webcam);
  void draw_overlay(int webcam);
  void for_webcam(int webcam);

  application_state &state_;
  ui_controls const &controls_;
  void *mock_data_;
};
WARNINGS_ON

} // namespace frank::video
