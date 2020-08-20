#pragma once

#include "ui/protected_cvui.h"

#include "opencv/opencv_window.h"
#include "ui/application_state.h"

namespace frank::video {

void other_window(EnhancedWindow &statistics, application_state &state,
                  opencv_window &window);

} // namespace frank::video
