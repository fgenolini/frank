#include "config.h"

WARNINGS_OFF
#include <iostream>
WARNINGS_ON

#include "application.h"
#include "videoConfig.h"

#ifdef _TEST_MAIN_2_
namespace test::frank {

int main(int argc, char const *argv[], ::frank::video::application &app)
#else
static ::frank::video::application app{};

int main(int argc, char const *argv[])
#endif
{
  std::cerr << argv[0] << " v" << VIDEO_VERSION_MAJOR << "."
            << VIDEO_VERSION_MINOR << '\n';
  app.run(argc, argv);
  return argc;
}

#ifdef _TEST_MAIN_2_
} // namespace test::frank
#endif
