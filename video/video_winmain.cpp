#include <cstdlib>
#include <cstring>
#include <iostream>

#include "config.h"
#include <windows.h>

#include "run_application.h"
#include "videoConfig.h"

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  frank::video::run_application();
}
