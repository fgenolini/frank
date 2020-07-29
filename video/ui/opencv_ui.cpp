#include "config.h"

WARNINGS_OFF
#include <cstdlib>
#include <vector>
WARNINGS_ON

#include "device/input_device.h"
#include "opencv_ui.h"
#include "video_gui.h"

namespace frank::video {

bool opencv_with_webcams(std::vector<input_device> &connected_webcams) {
  video_gui gui{(int)connected_webcams.size()};
  return gui.loop(connected_webcams);
}

[[noreturn]] void opencv_ui(std::vector<input_device> &connected_webcams) {
  if (opencv_with_webcams(connected_webcams)) {
    exit(EXIT_SUCCESS);
  } else {
    exit(EXIT_FAILURE);
  }
}

} // namespace frank::video
