#include "config.h"

#if !defined(_DO_NOTHING_EXIT_)
WARNINGS_OFF
#include <cstdlib>
WARNINGS_ON
#else
#include "test/do_nothing_exit.h"
#endif

#include "testable_exit.h"

namespace frank::video {

#if !defined(_DO_NOTHING_EXIT_)
NO_RETURN void exit(int result) { ::exit(result); };
#else
void exit(int result) { test::frank::do_nothing_exit(result); };
#endif

} // namespace frank::video
