#include "config.h"

WARNINGS_OFF
#include <iostream>
WARNINGS_ON

#include "run_application.h"
#include "videoConfig.h"

#ifdef _TEST_MAIN_2_
namespace test::frank {

int main(int argc, char const *argv[], void *mock_data)
#else
static void *mock_data{};

int main(int argc, char const *argv[])
#endif
{
  std::cerr << argv[0] << " v" << VIDEO_VERSION_MAJOR << "."
            << VIDEO_VERSION_MINOR << '\n';
  ::frank::video::run_application(argc, argv, mock_data);
  return argc;
}

#ifdef _TEST_MAIN_2_
} // namespace test::frank
#endif
