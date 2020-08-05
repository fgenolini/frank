#include "config.h"

WARNINGS_OFF
#include <windows.h>
WARNINGS_ON

#include "run_application.h"

#ifdef _TEST_WINMAIN_4_
namespace test::frank {

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int show_option,
                     void *mock_data)
#else
static void *mock_data{};

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int show_option)
#endif
{
  ::frank::video::run_application(0, nullptr, mock_data);
  return show_option;
}

#ifdef _TEST_WINMAIN_4_
} // namespace test::frank
#endif
