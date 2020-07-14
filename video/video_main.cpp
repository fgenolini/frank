#include <cstdlib>
#include <cstring>
#include <iostream>

#include "run_application.h"
#include "videoConfig.h"

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cerr << argv[0] << " v" << VIDEO_VERSION_MAJOR << "."
              << VIDEO_VERSION_MINOR << '\n';
    std::cerr << "Usage: " << argv[0] << " number_of_video_inputs\n";
    exit(EXIT_FAILURE);
  }

  frank::video::run_application(argc, argv);
}
