#pragma once

#include "config.h"

WARNINGS_OFF
#include <memory>
#include <vector>
WARNINGS_ON

#include "device/input_device.h"
#include "file/file_dialogs.h"
#include "ui/cvui_init.h"
#include "ui/ui_controls.h"

namespace frank::video {

// Abstract interface for a user interface, such as a windowed GUI
class user_interface {
public:
  virtual ~user_interface() = default;

  virtual void loop(std::vector<input_device> const &connected_webcams) = 0;
};

class user_interface_factory {
public:
  virtual ~user_interface_factory();

  virtual std::unique_ptr<user_interface> make(int webcam_count,
                                               cvui_init *initialise_windows,
                                               ui_controls &controls,
                                               file_dialogs &dialogs);
};

} // namespace frank::video
