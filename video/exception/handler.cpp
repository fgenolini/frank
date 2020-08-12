#include "config.h"

WARNINGS_OFF
#include <exception>
#include <iostream>
WARNINGS_ON

#include "handler.h"

namespace frank::video {

NO_RETURN
void all_exceptions_handler() {
  std::cerr << "Unhandled exception\n";
  std::abort();
}

} // namespace frank::video
