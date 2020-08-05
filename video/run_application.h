#pragma once

namespace frank::video {

#if !defined(_TEST_MAIN_2_) && !defined(_TEST_WINMAIN_4_) &&                   \
    !defined(_TEST_RUN_APPLICATION_2_)
[[noreturn]]
#endif
extern void
run_application(int argc = 0, char const *argv[] = nullptr);

} // namespace frank::video
