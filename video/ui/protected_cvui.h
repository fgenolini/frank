#pragma once

#include "config.h"

WARNINGS_OFF
#include <opencv2/opencv.hpp>

#define CVUI_DISABLE_COMPILATION_NOTICES
#include "cvui.h"

// EnhancedWindow.h must be included after cvui.h to provide EnhancedWindow
#include "EnhancedWindow.h"
WARNINGS_ON
