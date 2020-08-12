#include "config.h"

WARNINGS_OFF
#include <exception>
#include <iostream>
WARNINGS_ON

#include "handler.h"

namespace frank::video {

NO_RETURN
void all_exceptions_handler(std::exception const *caught_exception,
                            void *mock_data) noexcept {
  if (!caught_exception) {
    std::cerr << "Unhandled exception\n";
  } else {
    std::cerr << "Unhandled exception: " << caught_exception->what() << '\n';
  }

  abort(mock_data);
}

NO_RETURN
void unhandled_exception_handler() noexcept {
  all_exceptions_handler(nullptr, nullptr);
}

} // namespace frank::video
