#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "ui/protected_cvui.h"

#include "opencv/opencv_window.h"
#include "ui/application_state.h"

namespace frank::video {

void main_window(EnhancedWindow &settings, EnhancedWindow &statistics,
                 application_state &state, opencv_window &window,
                 void *mock_data = nullptr);

} // namespace frank::video
