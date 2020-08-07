#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "device/input_device.h"
#include "run_ui.h"
#include "test/testable_exit.h"
#include "user_interface.h"

namespace frank::video {

void interface_with_user(std::vector<input_device> const &connected_webcams,
                         user_interface_factory make_ui, void *mock_data) {
  auto ui = make_ui((int)connected_webcams.size(), mock_data);
  ui->loop(connected_webcams);
}

NO_RETURN
void run_ui(std::vector<input_device> const &connected_webcams,
            user_interface_factory mocked_factory, void *mock_data) {
  auto make_ui = make_user_interface;
  if (mocked_factory)
    make_ui = mocked_factory;

  interface_with_user(connected_webcams, make_ui, mock_data);
  exit(EXIT_SUCCESS, mock_data);
}

} // namespace frank::video
