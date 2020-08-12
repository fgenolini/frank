#include "config.h"

#ifndef _TEST_MAIN_SETTINGS_WINDOW_DRAW_4_
#error _TEST_MAIN_SETTINGS_WINDOW_DRAW_4_ not defined
#endif

WARNINGS_OFF
#include <iostream>
#include <vector>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "device/input_device.h"
#include "ui/main_settings_window.h"

namespace test::frank {

struct draw_mock {
  void begin_column() { ++begin_column_count; }
  void begin_row() { ++begin_row_count; }

  bool button() {
    ++button_count;
    return false;
  }

  bool checkbox() {
    ++checkbox_count;
    return false;
  }

  void end_column() { ++end_column_count; }
  void end_row() { ++end_row_count; }

  void text() { ++text_count; }

  bool trackbar_double() {
    ++trackbar_double_count;
    return false;
  }

  bool trackbar_int() {
    ++trackbar_int_count;
    return false;
  }

  int begin_column_count{};
  int begin_row_count{};
  int button_count{};
  int checkbox_count{};
  int end_column_count{};
  int end_row_count{};
  int text_count{};
  int trackbar_double_count{};
  int trackbar_int_count{};
};

} // namespace test::frank

namespace frank::video {

bool button_command::execute(int, int, std::string const &,
                             void *mock_data) const {
  auto mock = static_cast<test::frank::draw_mock *>(mock_data);
  return mock->button();
}

bool checkbox_command::execute(std::string const &, bool *returned_value,
                               void *mock_data) const {
  auto mock = static_cast<test::frank::draw_mock *>(mock_data);
  auto result = mock->checkbox();
  if (returned_value)
    *returned_value = result;
  return result;
}

void layout_command::begin_column(void *mock_data) const {
  auto mock = static_cast<test::frank::draw_mock *>(mock_data);
  mock->begin_column();
}

void layout_command::begin_row(void *mock_data) const {
  auto mock = static_cast<test::frank::draw_mock *>(mock_data);
  mock->begin_row();
}

void layout_command::end_column(void *mock_data) const {
  auto mock = static_cast<test::frank::draw_mock *>(mock_data);
  mock->end_column();
}

void layout_command::end_row(void *mock_data) const {
  auto mock = static_cast<test::frank::draw_mock *>(mock_data);
  mock->end_row();
}

void text_command::execute(std::string const &, void *mock_data) const {
  auto mock = static_cast<test::frank::draw_mock *>(mock_data);
  mock->text();
}

bool trackbar_double_command::execute(int, double *returned_value, double,
                                      double, void *mock_data) const {
  auto mock = static_cast<test::frank::draw_mock *>(mock_data);
  auto result = mock->trackbar_double();
  if (returned_value)
    *returned_value = result;
  return result;
}

bool trackbar_int_command::execute(int, int *returned_value, int, int,
                                   void *mock_data) const {
  auto mock = static_cast<test::frank::draw_mock *>(mock_data);
  auto result = mock->trackbar_int();
  if (returned_value)
    *returned_value = result;
  return result;
}

} // namespace frank::video

SCENARIO("frank video main settings window draw 4",
         "[main_settings_window_draw_4]") {
  GIVEN("main settings window minimised") {
    constexpr auto SETTINGS_MINIMISED = true;

    WHEN("draw") {
      frank::video::button_command button_mock{};
      frank::video::checkbox_command checkbox_mock{};
      frank::video::layout_command layout_mock{};
      frank::video::text_command text_mock{};
      frank::video::trackbar_double_command trackbar_double_mock{};
      frank::video::trackbar_int_command trackbar_int_mock{};
      std::vector<frank::video::input_device> devices{};
      std::vector<bool> has_webcams{true, false, false, false};
      std::vector<cv::String> images{};
      test::frank::draw_mock mock{};
      frank::video::main_settings_window window(
          button_mock, checkbox_mock, layout_mock, text_mock,
          trackbar_double_mock, trackbar_int_mock, nullptr, nullptr, nullptr,
          nullptr, nullptr, nullptr, &mock);

      window.draw(SETTINGS_MINIMISED, devices, has_webcams, images);

      THEN("no GUI control is shown") {
        REQUIRE(mock.begin_column_count == 0);
        REQUIRE(mock.begin_row_count == 0);
        REQUIRE(mock.button_count == 0);
        REQUIRE(mock.checkbox_count == 0);
        REQUIRE(mock.end_column_count == 0);
        REQUIRE(mock.end_row_count == 0);
        REQUIRE(mock.text_count == 0);
        REQUIRE(mock.trackbar_double_count == 0);
        REQUIRE(mock.trackbar_int_count == 0);
      }
    }
  }
  GIVEN("main settings window not minimised") {
    constexpr auto SETTINGS_NOT_MINIMISED = false;

    WHEN("draw") {
      frank::video::button_command button_mock{};
      frank::video::checkbox_command checkbox_mock{};
      frank::video::layout_command layout_mock{};
      frank::video::text_command text_mock{};
      frank::video::trackbar_double_command trackbar_double_mock{};
      frank::video::trackbar_int_command trackbar_int_mock{};
      std::vector<frank::video::input_device> devices{};
      std::vector<bool> has_webcams{true, false, false, false};
      std::vector<cv::String> images{};
      test::frank::draw_mock mock{};
      frank::video::main_settings_window window(
          button_mock, checkbox_mock, layout_mock, text_mock,
          trackbar_double_mock, trackbar_int_mock, nullptr, nullptr, nullptr,
          nullptr, nullptr, nullptr, &mock);

      window.draw(SETTINGS_NOT_MINIMISED, devices, has_webcams, images);

      THEN("4 buttons are shown") { REQUIRE(mock.button_count == 4); }

      THEN("1 checkbox is shown") { REQUIRE(mock.checkbox_count == 1); }

      THEN("46 text labels are shown") { REQUIRE(mock.text_count == 46); }

      THEN("no double trackbar is shown") {
        REQUIRE(mock.trackbar_double_count == 0);
      }

      THEN("2 int trackbars are shown") {
        REQUIRE(mock.trackbar_int_count == 2);
      }

      THEN("layout was performed") {
        REQUIRE(mock.begin_column_count > 0);
        REQUIRE(mock.begin_row_count > 0);
        REQUIRE(mock.end_column_count > 0);
        REQUIRE(mock.end_row_count > 0);
      }
    }
  }
}
