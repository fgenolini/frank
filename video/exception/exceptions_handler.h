#pragma once

#include "config.h"

WARNINGS_OFF
#include <exception>
WARNINGS_ON

#include "test/testable_exit.h"

namespace frank::video {

NO_RETURN
extern void exceptions_handler(std::exception const *caught_exception = nullptr,
                               void *mock_data = nullptr) noexcept;

NO_RETURN
extern void unhandled_exception_handler() noexcept;

} // namespace frank::video
