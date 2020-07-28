#pragma once

#include "config.h"

#if defined(WIN32)
#pragma warning(push, 0)
#pragma warning(disable : 4365)
#endif

#include <opencv2/opencv.hpp>

#define CVUI_DISABLE_COMPILATION_NOTICES
#include "cvui.h"

// EnhancedWindow.h must be included after cvui.h to provide EnhancedWindow
#include "EnhancedWindow.h"

#if defined(WIN32)
#pragma warning(pop)
#endif
