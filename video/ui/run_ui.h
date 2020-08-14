#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "device/input_device.h"
#include "test/testable_exit.h"
#include "ui/user_interface.h"

namespace frank::video {

NO_RETURN
extern void run_ui(std::vector<input_device> const &connected_webcams,
                   cvui_init const &initialise_windows,
                   user_interface_factory mocked_factory = nullptr,
                   void *mock_data = nullptr);

} // namespace frank::video
