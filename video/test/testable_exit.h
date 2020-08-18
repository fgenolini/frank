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
  [[maybe_unused]] void *mock_data_;
};

class exiter {
public:
  exiter(void *mock_data = nullptr);

  NO_RETURN void exit(int result) noexcept;

private:
  [[maybe_unused]] void *mock_data_;
};

} // namespace frank::video
