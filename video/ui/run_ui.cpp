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
                         user_interface_factory make_ui,
                         cvui_init const &initialise_windows, void *mock_data) {
  auto ui =
      make_ui((int)connected_webcams.size(), initialise_windows, mock_data);
  ui->loop(connected_webcams);
}

NO_RETURN
void run_ui(std::vector<input_device> const &connected_webcams,
            cvui_init const &initialise_windows,
            user_interface_factory mocked_factory, void *mock_data) {
  auto make_ui = make_user_interface;
  if (mocked_factory)
    make_ui = mocked_factory;

  interface_with_user(connected_webcams, make_ui, initialise_windows,
                      mock_data);
  exit(EXIT_SUCCESS, mock_data);
}

} // namespace frank::video
