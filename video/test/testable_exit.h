#pragma once

#if !defined(_DO_NOTHING_EXIT_)
#define NO_RETURN [[noreturn]]
#else
#define NO_RETURN
#endif

namespace frank::video {

class aborter {
public:
  aborter(void *mock_data = nullptr);

  NO_RETURN void abort() noexcept;

private:
  WARNINGS_OFF
  void *mock_data_;
  WARNINGS_ON
};

class exiter {
public:
  exiter(void *mock_data = nullptr);

  NO_RETURN void exit(int result) noexcept;

private:
  WARNINGS_OFF
  void *mock_data_;
  WARNINGS_ON
};

} // namespace frank::video
