#pragma once

#include "config.h"

WARNINGS_OFF
#include <memory>
#include <vector>
WARNINGS_ON

#include "device/input_device.h"

namespace frank::video {

// Abstract interface for a user interface, such as a windowed GUI
class user_interface {
public:
  virtual ~user_interface() = default;

  virtual int loop(std::vector<input_device> &inputs) = 0;
};

std::unique_ptr<user_interface> make_user_interface(int webcam_count,
                                                    void *test_mock = nullptr);

typedef std::unique_ptr<user_interface> (*user_interface_factory)(
    int webcam_count, void *test_mock);

} // namespace frank::video
