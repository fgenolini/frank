#include "config.h"

WARNINGS_OFF
#include <exception>
#include <iostream>
WARNINGS_ON

#include "exceptions_handler.h"

namespace frank::video {

NO_RETURN
void exceptions_handler(std::exception const *caught_exception,
                        void *mock_data) noexcept {
  if (caught_exception)
    std::cerr << "Unhandled exception: " << caught_exception->what() << '\n';
  else
    std::cerr << "Unhandled exception\n";

  abort(mock_data);
}

NO_RETURN
void unhandled_exception_handler() noexcept {
  exceptions_handler(nullptr, nullptr);
}

} // namespace frank::video
