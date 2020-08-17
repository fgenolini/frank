#pragma once

#include "config.h"

WARNINGS_OFF
#include <exception>
WARNINGS_ON

#include "test/testable_exit.h"

namespace frank::video {

class exceptions {
public:
  exceptions(aborter *injected_aborter = nullptr);
  virtual ~exceptions();

  NO_RETURN
  virtual void
  handler(std::exception const *caught_exception = nullptr) noexcept;

private:
  aborter *aborter_;
};

extern aborter *global_injected_aborter;

NO_RETURN
extern void unhandled_exception_handler() noexcept;

} // namespace frank::video
