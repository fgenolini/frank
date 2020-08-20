#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "ui/protected_cvui.h"

#include "file/file_dialogs.h"
#include "opencv/opencv_window.h"
#include "ui/application_state.h"
#include "ui/ui_controls.h"

namespace frank::video {

class main_window {
public:
  main_window(ui_controls &controls, file_dialogs &dialogs);
  virtual ~main_window();

  virtual void draw(EnhancedWindow &settings, EnhancedWindow &statistics,
                    application_state &state, opencv_window &window);

protected:
  file_dialogs &dialogs_;
  ui_controls &controls_;

private:
  void first_row(bool has_webcam, opencv_window &window, bool &should_exit);
  void first_time_display(int webcam_index);
  bool quit_selected(opencv_window &window);
  void settings_display(EnhancedWindow &settings, cv::Mat frame,
                        application_state &state);
};

} // namespace frank::video
