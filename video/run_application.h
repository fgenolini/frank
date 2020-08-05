#pragma once

#include "test/testable_exit.h"

namespace frank::video {

NO_RETURN
extern void run_application(int argc = 0, char const *argv[] = nullptr,
                            void *mock_data = nullptr);

} // namespace frank::video
