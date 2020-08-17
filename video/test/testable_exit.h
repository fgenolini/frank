#pragma once

#if !defined(_DO_NOTHING_EXIT_)
#define NO_RETURN [[noreturn]]
#else
#define NO_RETURN
#endif

namespace frank::video {

class aborter {
public:
  virtual ~aborter();

  NO_RETURN
  virtual void abort() noexcept;
};

class exiter {
public:
  virtual ~exiter();

  NO_RETURN
  virtual void exit(int result) noexcept;
};

} // namespace frank::video
