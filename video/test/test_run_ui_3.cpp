#include "config.h"

#ifndef _TEST_RUN_UI_3_
#error _TEST_RUN_UI_3_ not defined
#endif

WARNINGS_OFF
#include <iostream>

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
WARNINGS_ON

#include "ui/run_ui.h"
#include "ui/user_interface.h"

namespace test::frank {

WARNINGS_OFF
class mock_user_interface
    : public trompeloeil::mock_interface<::frank::video::user_interface> {
public:
  IMPLEMENT_MOCK1(loop);

  int exit_count{};
  int exit_value{3};
};

class fake_user_interface : public virtual ::frank::video::user_interface {
public:
  fake_user_interface(mock_user_interface *mock) : mock_(mock) {}

  void loop(std::vector<::frank::video::input_device> const &inputs) override {
    if (!mock_)
      return;

    mock_->loop(inputs);
  }

private:
  mock_user_interface *mock_;
};
WARNINGS_ON

void do_nothing_exit(int result, void *mock_data) {
  if (!mock_data)
    return;

  auto mock = static_cast<mock_user_interface *>(mock_data);
  ++(mock->exit_count);
  mock->exit_value = result;
}

} // namespace test::frank

namespace frank::video {

std::unique_ptr<user_interface> make_user_interface(int, void *mock_data) {
  if (!mock_data)
    return std::make_unique<::test::frank::fake_user_interface>(nullptr);

  auto mock = static_cast<::test::frank::mock_user_interface *>(mock_data);
  return std::make_unique<::test::frank::fake_user_interface>(mock);
}

} // namespace frank::video

SCENARIO("frank video run ui 3", "[run_ui_3]") {
  GIVEN("user interface is run") {
    WHEN("no connected webcam") {
      std::vector<frank::video::input_device> no_device{};
      test::frank::mock_user_interface mock{};
      REQUIRE_CALL(mock, loop(no_device));

      frank::video::run_ui(no_device, frank::video::make_user_interface, &mock);

      THEN("exit is called") { REQUIRE(mock.exit_count == 1); }

      THEN("exit returns EXIT_SUCCESS") {
        REQUIRE(mock.exit_value == EXIT_SUCCESS);
      }
    }
    WHEN("one connected webcam") {
      std::vector<frank::video::input_device> one_device{
          frank::video::input_device()};
      test::frank::mock_user_interface mock{};
      REQUIRE_CALL(mock, loop(one_device));

      frank::video::run_ui(one_device, frank::video::make_user_interface,
                           &mock);

      THEN("exit is called") { REQUIRE(mock.exit_count == 1); }

      THEN("exit returns EXIT_SUCCESS") {
        REQUIRE(mock.exit_value == EXIT_SUCCESS);
      }
    }
  }
}