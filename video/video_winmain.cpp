#include "config.h"

WARNINGS_OFF
#include <windows.h>
WARNINGS_ON

#include "application.h"

#ifdef _TEST_WINMAIN_4_
namespace test::frank {

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int show_option,
                     ::frank::video::application &app)
#else
static ::frank::video::application app{};

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int show_option)
#endif
{
  app.run();
  return show_option;
}

#ifdef _TEST_WINMAIN_4_
} // namespace test::frank
#endif
