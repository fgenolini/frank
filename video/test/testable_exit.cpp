#include "config.h"

#if !defined(_DO_NOTHING_EXIT_)
WARNINGS_OFF
#include <cstdlib>
WARNINGS_ON
#endif

#include "test/testable_exit.h"

namespace frank::video {

aborter::aborter(void *mock_data) : mock_data_(mock_data) {}

NO_RETURN void aborter::abort() noexcept {
  if (mock_data_) {
  }

#if !defined(_DO_NOTHING_EXIT_)
  ::abort();
#endif
}

exiter::exiter(void *mock_data) : mock_data_(mock_data) {}

#if !defined(_DO_NOTHING_EXIT_)
NO_RETURN void exiter::exit(int result) noexcept {
  if (mock_data_) {
  }

  ::exit(result);
}
#else
NO_RETURN void exiter::exit(int) noexcept {
  if (mock_data_) {
  }
}
#endif

} // namespace frank::video
