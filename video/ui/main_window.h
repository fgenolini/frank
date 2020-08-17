#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "ui/protected_cvui.h"

#include "opencv/opencv_window.h"
#include "ui/application_state.h"

namespace frank::video {

class main_window {
public:
  virtual ~main_window();

  virtual void draw(EnhancedWindow &settings, EnhancedWindow &statistics,
                    application_state &state, opencv_window &window);
};

} // namespace frank::video
