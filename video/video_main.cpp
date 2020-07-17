#include <cstdlib>
#include <cstring>
#include <iostream>

#include "run_application.h"
#include "videoConfig.h"

int main(int argc, char const *argv[]) {
  std::cerr << argv[0] << " v" << VIDEO_VERSION_MAJOR << "."
            << VIDEO_VERSION_MINOR << '\n';
  frank::video::run_application(argc, argv);
}
