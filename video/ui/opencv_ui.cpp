#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "device/input_device.h"
#include "test/testable_exit.h"
#include "user_interface.h"

namespace frank::video {

auto opencv_with_webcams(std::vector<input_device> &connected_webcams) {
  auto gui = make_user_interface((int)connected_webcams.size());
  return gui->loop(connected_webcams);
}

NO_RETURN
void opencv_ui(std::vector<input_device> &connected_webcams) {
  auto result = EXIT_SUCCESS;
  if (opencv_with_webcams(connected_webcams) != (int)connected_webcams.size()) {
    result = EXIT_FAILURE;
  }

  exit(result);
}

} // namespace frank::video
