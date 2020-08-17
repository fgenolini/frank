#include "config.h"

WARNINGS_OFF
#include <exception>
#include <iostream>
WARNINGS_ON

#include "exception/exceptions.h"

namespace frank::video {

exceptions::exceptions(aborter *injected_aborter)
    : aborter_(injected_aborter) {}

exceptions::~exceptions() {}

WARNINGS_OFF

NO_RETURN void abort_noreturn(aborter *a) {
  // noreturn not guaranteed to be inherited by derived classes
  a->abort();
}

WARNINGS_ON

NO_RETURN
void exceptions::handler(std::exception const *caught_exception) noexcept {
  if (caught_exception)
    std::cerr << "Unhandled exception: " << caught_exception->what() << '\n';
  else
    std::cerr << "Unhandled exception\n";

  aborter default_aborter{};
  auto a = aborter_ ? aborter_ : &default_aborter;
  abort_noreturn(a);
}

aborter *global_injected_aborter{};

NO_RETURN
void unhandled_exception_handler() noexcept {
  exceptions ex{global_injected_aborter};
  ex.handler();
}

} // namespace frank::video
