#include "config.h"

#include <windows.h>

#include "run_application.h"

#ifdef _TEST_WINMAIN_4_
namespace test::frank {
#endif

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int show_option) {
  ::frank::video::run_application();
  return show_option;
}

#ifdef _TEST_WINMAIN_4_
} // namespace test::frank
#endif
