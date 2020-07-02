#include <cstdlib>
#include <iostream>

#include "videoConfig.h"
#include "list_devices.h"

int main(int argc, char const* argv[])
{
  if (argc < 2)
  {
    // report version
    std::cout << argv[0] << " v" << VIDEO_VERSION_MAJOR << "."
      << VIDEO_VERSION_MINOR << "\n";
    std::cout << "Usage: " << argv[0] << " number" << "\n";
    exit(EXIT_FAILURE);
  }

  if (!video::list_input_devices()) {
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
