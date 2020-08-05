#include "config.h"

#ifndef _TEST_OPENCV_UI_1_
#error _TEST_OPENCV_UI_1_ not defined
#endif

WARNINGS_OFF
#include <iostream>
WARNINGS_ON

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

#include "ui/opencv_ui.h"
#include "ui/user_interface.h"

namespace test::frank {

class mock_user_interface
    : public trompeloeil::mock_interface<::frank::video::user_interface> {
public:
  IMPLEMENT_MOCK1(loop);

  int exit_count{};
  int exit_value{3};
};

class fake_user_interface : public virtual ::frank::video::user_interface {
public:
  fake_user_interface(int webcam_count, mock_user_interface *mock)
      : webcam_count_(webcam_count), mock_(mock) {}

  int loop(std::vector<::frank::video::input_device> &inputs) override {
    if (!mock_) {
      std::cerr << "loop: no mock\n";
      return -1;
    }

    return mock_->loop(inputs);
  }

private:
  int webcam_count_;
  mock_user_interface *mock_;
};

void do_nothing_exit(int result, void *mock_data) {
  if (!mock_data) {
    std::cerr << "do_nothing_exit: no mock\n";
    return;
  }

  auto mock = static_cast<mock_user_interface *>(mock_data);
  ++(mock->exit_count);
  mock->exit_value = result;
}

} // namespace test::frank

namespace frank::video {

std::unique_ptr<user_interface> make_user_interface(int webcam_count,
                                                    void *mock_data) {
  if (!mock_data) {
    std::cerr << "make_user_interface: no mock\n";
    return std::make_unique<::test::frank::fake_user_interface>(webcam_count,
                                                                nullptr);
  }

  auto mock = static_cast<::test::frank::mock_user_interface *>(mock_data);
  return std::make_unique<::test::frank::fake_user_interface>(webcam_count,
                                                              mock);
}

} // namespace frank::video

SCENARIO("frank video opencv ui 1", "[run_opencv_ui_1]") {
  GIVEN("OpenCV UI") {
    WHEN("no connected webcam") {
      std::vector<frank::video::input_device> no_device{};
      test::frank::mock_user_interface mock{};
      auto device_count = 0;
      REQUIRE_CALL(mock, loop(no_device)).LR_RETURN(device_count);

      frank::video::opencv_ui(no_device, frank::video::make_user_interface,
                              &mock);

      THEN("exit is called") { REQUIRE(mock.exit_count == 1); }

      THEN("exit returns EXIT_SUCCESS") {
        REQUIRE(mock.exit_value == EXIT_SUCCESS);
      }
    }
    WHEN("one connected webcam") {
      std::vector<frank::video::input_device> one_device{
          frank::video::input_device()};
      test::frank::mock_user_interface mock{};
      auto device_count = 1;
      REQUIRE_CALL(mock, loop(one_device)).LR_RETURN(device_count);

      frank::video::opencv_ui(one_device, frank::video::make_user_interface,
                              &mock);

      THEN("exit is called") { REQUIRE(mock.exit_count == 1); }

      THEN("exit returns EXIT_SUCCESS") {
        REQUIRE(mock.exit_value == EXIT_SUCCESS);
      }
    }
    WHEN("wrong webcam count") {
      std::vector<frank::video::input_device> one_device{
          frank::video::input_device()};
      test::frank::mock_user_interface mock{};
      auto wrong_device_count = 42;
      REQUIRE_CALL(mock, loop(one_device)).LR_RETURN(wrong_device_count);

      frank::video::opencv_ui(one_device, frank::video::make_user_interface,
                              &mock);

      THEN("exit is called") { REQUIRE(mock.exit_count == 1); }

      THEN("exit returns EXIT_FAILURE") {
        REQUIRE(mock.exit_value == EXIT_FAILURE);
      }
    }
  }
}
