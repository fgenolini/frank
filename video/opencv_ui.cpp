#include <cstdlib>

#include <opencv2/opencv.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include "opencv_ui.h"

namespace frank::video
{

  [[noreturn]] void opencv_ui()
  {
    constexpr auto ESCAPE_KEY = 27;
    constexpr auto SHORT_DELAY = 20;
    constexpr auto WINDOW_NAME = "Frank video";

    auto frame = cv::Mat(200, 500, CV_8UC3);
    cvui::init(WINDOW_NAME);
    while (true)
    {
      // Fill the frame with a nice color
      frame = cv::Scalar(49, 52, 49);
      auto button_clicked = cvui::button(frame, 110, 80, "Quit");
      if (button_clicked)
      {
        exit(EXIT_SUCCESS);
      }

      cvui::imshow(WINDOW_NAME, frame);
      if (cv::waitKey(SHORT_DELAY) == ESCAPE_KEY)
      {
        exit(EXIT_SUCCESS);
      }
    }

    exit(EXIT_SUCCESS);
  }

} // namespace frank::video
