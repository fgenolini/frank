#pragma once

#include "test/testable_exit.h"

namespace frank::video {

class application {
public:
  application(void *mock_data = nullptr);

  NO_RETURN
  void run(int argc = 0, char const *argv[] = nullptr);

private:
  NO_RETURN
  void webcam_viewer() noexcept(false);

  void *mock_data_;
};

} // namespace frank::video
