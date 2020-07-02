#include <cstdlib>
#include <iostream>

#include "videoConfig.h"
#include "list_devices.h"
#include "opencv_ui.h"

int main(int argc, char const *argv[])
{
  if (argc < 2)
  {
    std::cout << argv[0] << " v" << VIDEO_VERSION_MAJOR << "."
              << VIDEO_VERSION_MINOR << "\n";
    std::cout << "Usage: " << argv[0] << " number"
              << "\n";
    exit(EXIT_FAILURE);
  }

  if (!frank::video::list_input_devices())
  {
    exit(EXIT_FAILURE);
  }

  frank::video::opencv_ui();
}
