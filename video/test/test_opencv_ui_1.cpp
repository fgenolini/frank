#include "config.h"

#ifndef _TEST_OPENCV_UI_1_
#error _TEST_OPENCV_UI_1_ not defined
#endif

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

#include "ui/opencv_ui.h"
#include "ui/user_interface.h"

namespace test::frank {

static auto mock_exit_called = false;

static auto mock_exit_value = 3;

class mock_user_interface
    : public trompeloeil::mock_interface<::frank::video::user_interface> {
public:
  IMPLEMENT_MOCK1(loop);
};

static mock_user_interface *global_mock_{};

class fake_user_interface : public virtual ::frank::video::user_interface {
public:
  fake_user_interface(int webcam_count) : webcam_count_(webcam_count) {}

  int loop(std::vector<::frank::video::input_device> &inputs) override {
    return global_mock_->loop(inputs);
  }

private:
  int webcam_count_;
};

void do_nothing_exit(int result) {
  mock_exit_called = true;
  mock_exit_value = result;
}

} // namespace test::frank

namespace frank::video {

std::unique_ptr<user_interface> make_user_interface(int webcam_count) {
  return std::make_unique<::test::frank::fake_user_interface>(webcam_count);
}

} // namespace frank::video

SCENARIO("frank video opencv ui 1", "[run_opencv_ui_1]") {
  GIVEN("OpenCV UI") {
    WHEN("no connected webcam") {
      ::test::frank::mock_exit_called = false;
      ::test::frank::mock_exit_value = 3;
      std::vector<frank::video::input_device> no_device{};
      test::frank::mock_user_interface mock{};
      test::frank::global_mock_ = &mock;
      auto device_count = 0;
      REQUIRE_CALL(mock, loop(no_device)).LR_RETURN(device_count);

      frank::video::opencv_ui(no_device);

      THEN("exit is called") { REQUIRE(test::frank::mock_exit_called == true); }

      THEN("exit returns EXIT_SUCCESS") {
        REQUIRE(test::frank::mock_exit_value == EXIT_SUCCESS);
      }
    }
    WHEN("one connected webcam") {
      ::test::frank::mock_exit_called = false;
      ::test::frank::mock_exit_value = 3;
      std::vector<frank::video::input_device> one_device{
          frank::video::input_device()};
      test::frank::mock_user_interface mock{};
      test::frank::global_mock_ = &mock;
      auto device_count = 1;
      REQUIRE_CALL(mock, loop(one_device)).LR_RETURN(device_count);

      frank::video::opencv_ui(one_device);

      THEN("exit is called") { REQUIRE(test::frank::mock_exit_called == true); }

      THEN("exit returns EXIT_SUCCESS") {
        REQUIRE(test::frank::mock_exit_value == EXIT_SUCCESS);
      }
    }
    WHEN("wrong webcam count") {
      ::test::frank::mock_exit_called = false;
      ::test::frank::mock_exit_value = 3;
      std::vector<frank::video::input_device> one_device{
          frank::video::input_device()};
      test::frank::mock_user_interface mock{};
      test::frank::global_mock_ = &mock;
      auto wrong_device_count = 42;
      REQUIRE_CALL(mock, loop(one_device)).LR_RETURN(wrong_device_count);

      frank::video::opencv_ui(one_device);

      THEN("exit is called") { REQUIRE(test::frank::mock_exit_called == true); }

      THEN("exit returns EXIT_FAILURE") {
        REQUIRE(test::frank::mock_exit_value == EXIT_FAILURE);
      }
    }
  }
}
