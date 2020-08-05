#pragma once

#include "config.h"

WARNINGS_OFF
#include <vector>
WARNINGS_ON

#include "device/input_device.h"
#include "test/testable_exit.h"
#include "user_interface.h"

namespace frank::video {

NO_RETURN
extern void opencv_ui(std::vector<input_device> &connected_webcams,
                      user_interface_factory factory_from_test = nullptr,
                      void *mock_data = nullptr);

} // namespace frank::video
