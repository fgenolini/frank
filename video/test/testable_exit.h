#pragma once

#if !defined(_DO_NOTHING_EXIT_)
#define NO_RETURN [[noreturn]]
#else
#define NO_RETURN
#endif

namespace frank::video {

NO_RETURN
extern void abort(void *mock_data = nullptr) noexcept;

NO_RETURN
extern void exit(int result, void *mock_data = nullptr) noexcept;

} // namespace frank::video
