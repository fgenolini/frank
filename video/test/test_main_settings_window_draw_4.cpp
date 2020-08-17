#include "config.h"

#ifndef _TEST_MAIN_SETTINGS_WINDOW_DRAW_4_
#error _TEST_MAIN_SETTINGS_WINDOW_DRAW_4_ not defined
#endif

#ifndef _FAKE_FILE_DIALOGS_
#error _FAKE_FILE_DIALOGS_ not defined
#endif

WARNINGS_OFF
#include <iostream>
#include <string>
#include <vector>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "device/input_device.h"
#include "file/file_dialogs.h"
#include "ui/main_settings_window.h"

namespace test::frank {

class mock_file_dialogs : public ::frank::video::file_dialogs {
public:
  ~mock_file_dialogs() override {}

  std::vector<std::string>
  open_file(std::string const &, std::string const &,
            std::vector<std::string> const &) override {
    ++open_file_count;
    if (open_file_count > 2)
      return std::vector<std::string>();

    return std::vector<std::string>{"test file"};
  }

  int open_file_count{};
};

class mock_ui_controls : public ::frank::video::ui_controls {
public:
  ~mock_ui_controls() override {}

  void begin_column() override { ++begin_column_count; }

  void begin_row() override { ++begin_row_count; }

  bool button(int, int, std::string const &label) override {
    ++button_count;
    if (label.compare(::frank::video::OVERLAY_BUTTON_LABEL) == 0)
      return true;

    return false;
  }

  bool checkbox(std::string const &, bool *) override {
    ++checkbox_count;
    return false;
  }

  void end_column() override { ++end_column_count; }

  void end_row() override { ++end_row_count; }

  void text(std::string const &) override { ++text_count; }

  bool trackbar_double(int, double *, double, double) override {
    ++trackbar_double_count;
    return false;
  }

  bool trackbar_int(int, int *, int, int) override {
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

file_dialogs::~file_dialogs() {}

std::vector<std::string>
file_dialogs::open_file(std::string const &, std::string const &,
                        std::vector<std::string> const &) {
  return std::vector<std::string>();
}

ui_controls::~ui_controls() {}

void ui_controls::begin_column() {}

void ui_controls::begin_row() {}

bool ui_controls::button(int, int, std::string const &) { return false; }

bool ui_controls::checkbox(std::string const &, bool *) { return false; }

void ui_controls::end_column() {}

void ui_controls::end_row() {}

void ui_controls::text(std::string const &) {}

bool ui_controls::trackbar_double(int, double *, double, double) {
  return false;
}

bool ui_controls::trackbar_int(int, int *, int, int) { return false; }

} // namespace frank::video

SCENARIO("frank video main settings window draw 4",
         "[main_settings_window_draw_4]") {
  GIVEN("main settings window minimised") {
    constexpr auto SETTINGS_MINIMISED = true;

    WHEN("draw") {
      frank::video::application_state state{frank::video::MAXIMUM_VIDEO_COUNT};
      test::frank::mock_file_dialogs mocked_file_dialogs{};
      test::frank::mock_ui_controls mocked_ui_controls{};

      frank::video::main_settings_window window(mocked_ui_controls, state,
                                                mocked_file_dialogs);
      window.draw(SETTINGS_MINIMISED);

      THEN("no GUI control is shown") {
        REQUIRE(mocked_file_dialogs.open_file_count == 0);
        REQUIRE(mocked_ui_controls.begin_column_count == 0);
        REQUIRE(mocked_ui_controls.begin_row_count == 0);
        REQUIRE(mocked_ui_controls.button_count == 0);
        REQUIRE(mocked_ui_controls.checkbox_count == 0);
        REQUIRE(mocked_ui_controls.end_column_count == 0);
        REQUIRE(mocked_ui_controls.end_row_count == 0);
        REQUIRE(mocked_ui_controls.text_count == 0);
        REQUIRE(mocked_ui_controls.trackbar_double_count == 0);
        REQUIRE(mocked_ui_controls.trackbar_int_count == 0);
      }
    }
  }
  GIVEN("main settings window not minimised") {
    constexpr auto SETTINGS_NOT_MINIMISED = false;

    WHEN("draw") {
      frank::video::application_state state{frank::video::MAXIMUM_VIDEO_COUNT};
      state.overlay_images[0] = "test image";
      test::frank::mock_file_dialogs mocked_file_dialogs{};
      test::frank::mock_ui_controls mocked_ui_controls{};

      frank::video::main_settings_window window(mocked_ui_controls, state,
                                                mocked_file_dialogs);
      window.draw(SETTINGS_NOT_MINIMISED);

      THEN("open_file was called") {
        REQUIRE(mocked_file_dialogs.open_file_count > 0);
      }

      THEN("buttons are shown") {
        REQUIRE(mocked_ui_controls.button_count ==
                frank::video::MAXIMUM_VIDEO_COUNT);
      }

      THEN("checkboxes are shown") {
        REQUIRE(mocked_ui_controls.checkbox_count == 9);
      }

      THEN("text labels are shown") {
        REQUIRE(mocked_ui_controls.text_count == 46);
      }

      THEN("double trackbars are shown") {
        REQUIRE(mocked_ui_controls.trackbar_double_count ==
                frank::video::MAXIMUM_VIDEO_COUNT);
      }

      THEN("int trackbars are shown") {
        REQUIRE(mocked_ui_controls.trackbar_int_count == 2);
      }

      THEN("layout was performed") {
        REQUIRE(mocked_ui_controls.begin_column_count > 0);
        REQUIRE(mocked_ui_controls.begin_row_count > 0);
        REQUIRE(mocked_ui_controls.end_column_count > 0);
        REQUIRE(mocked_ui_controls.end_row_count > 0);
      }
    }
  }
}
