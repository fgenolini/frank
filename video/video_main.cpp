#include <cstdlib>
#include <cstring>
#include <iostream>

#include "config.h"
#ifdef WIN32
#include <windows.h>
#endif

#include "run_application.h"
#include "videoConfig.h"

#ifndef WIN32
int main(int argc, char const *argv[]) {
  std::cerr << argv[0] << " v" << VIDEO_VERSION_MAJOR << "."
            << VIDEO_VERSION_MINOR << '\n';
  frank::video::run_application(argc, argv);
}
#else
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  frank::video::run_application();
}
#endif
