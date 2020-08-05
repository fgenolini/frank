#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "device/input_device.h"
#include "test/testable_exit.h"
#include "user_interface.h"

namespace frank::video {

auto opencv_with_webcams(std::vector<input_device> &connected_webcams,
                         user_interface_factory factory, void *mock_data) {
  auto gui = factory((int)connected_webcams.size(), mock_data);
  return gui->loop(connected_webcams);
}

NO_RETURN
void opencv_ui(std::vector<input_device> &connected_webcams,
               user_interface_factory factory_from_test, void *mock_data) {
  auto ui_factory = make_user_interface;
  if (factory_from_test) {
    ui_factory = factory_from_test;
  }

  auto result = EXIT_SUCCESS;
  if (opencv_with_webcams(connected_webcams, ui_factory, mock_data) !=
      (int)connected_webcams.size()) {
    result = EXIT_FAILURE;
  }

  exit(result, mock_data);
}

} // namespace frank::video
