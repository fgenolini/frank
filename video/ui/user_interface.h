#pragma once

#include "config.h"

WARNINGS_OFF
#include <memory>
#include <string>
#include <vector>
WARNINGS_ON

#include "device/input_device.h"

namespace frank::video {

typedef void (*cvui_init)(const std::string window_names[], size_t window_count,
                          void *mock_data);

// Abstract interface for a user interface, such as a windowed GUI
class user_interface {
public:
  virtual ~user_interface() = default;

  virtual void loop(std::vector<input_device> const &inputs) = 0;
};

std::unique_ptr<user_interface>
make_user_interface(int webcam_count, cvui_init mock_init = nullptr,
                    void *mock_data = nullptr);

typedef std::unique_ptr<user_interface> (*user_interface_factory)(
    int webcam_count, cvui_init mock_init, void *mock_data);

} // namespace frank::video
