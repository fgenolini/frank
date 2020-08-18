#include "config.h"

WARNINGS_OFF
#include <exception>
#include <iostream>
WARNINGS_ON

#include "exception/exceptions.h"

namespace frank::video {

exceptions::exceptions(aborter *injected_aborter, void *mock_data)
    : aborter_(injected_aborter), mock_data_(mock_data) {}

NO_RETURN
void exceptions::handler(std::exception const *caught_exception) noexcept {
  if (caught_exception)
    std::cerr << "Unhandled exception: " << caught_exception->what() << '\n';
  else
    std::cerr << "Unhandled exception\n";

  aborter default_aborter{};
  auto a = aborter_ ? aborter_ : &default_aborter;
  a->abort();
}

aborter *global_injected_aborter{};

NO_RETURN
void unhandled_exception_handler() noexcept {
  exceptions ex{global_injected_aborter};
  ex.handler();
}

} // namespace frank::video
