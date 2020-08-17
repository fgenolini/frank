#include "config.h"

#ifndef _TEST_RUN_UI_3_
#error _TEST_RUN_UI_3_ not defined
#endif

WARNINGS_OFF
#include <cstdlib>
#include <iostream>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "ui/ui.h"
#include "ui/user_interface.h"

namespace test::frank {

class mock_exiter : public ::frank::video::exiter {
public:
  virtual void exit(int value) noexcept override {
    exit_called_ = true;
    exit_value_ = value;
  }

  bool exit_called() { return exit_called_; }

  int exit_value() { return exit_value_; }

private:
  bool exit_called_{};
  int exit_value_{};
};

class mock_video_gui : public ::frank::video::user_interface {
public:
  virtual ~mock_video_gui() override {}

  virtual void
  loop(std::vector<::frank::video::input_device> const &) override {
    loop_called_ = true;
  }

  bool loop_called() { return loop_called_; }

private:
  bool loop_called_{};
};

class mock_video_gui_holder : public ::frank::video::user_interface {
public:
  mock_video_gui_holder(mock_video_gui &mock) : mock_(mock) {}
  virtual ~mock_video_gui_holder() override {}

  virtual void
  loop(std::vector<::frank::video::input_device> const &devices) override {
    mock_.loop(devices);
  }

  bool loop_called() { return mock_.loop_called(); }

private:
  mock_video_gui &mock_;
};

class mock_user_interface_factory
    : public ::frank::video::user_interface_factory {
public:
  mock_user_interface_factory(std::unique_ptr<mock_video_gui_holder> &mock)
      : mock_(mock) {}
  ~mock_user_interface_factory() {}

  virtual std::unique_ptr<::frank::video::user_interface>
  make(int, ::frank::video::cvui_init *) override {
    make_called_ = true;
    return std::move(mock_);
  }

  bool make_called() { return make_called_; }

private:
  std::unique_ptr<mock_video_gui_holder> &mock_;
  bool make_called_{};
};

} // namespace test::frank

namespace frank::video {

user_interface_factory::~user_interface_factory() {}

std::unique_ptr<user_interface> user_interface_factory::make(int, cvui_init *) {
  return nullptr;
}

} // namespace frank::video

SCENARIO("frank video run ui 3", "[run_ui_3]") {
  GIVEN("user interface is run") {
    WHEN("no connected webcam") {
      std::vector<frank::video::input_device> no_device{};
      test::frank::mock_exiter mocked_exiter{};
      test::frank::mock_video_gui mocked_video_gui{};
      auto mocked_video_gui_holder =
          std::make_unique<test::frank::mock_video_gui_holder>(
              mocked_video_gui);
      test::frank::mock_user_interface_factory mocked_user_interface_factory(
          mocked_video_gui_holder);

      frank::video::ui ui_test(no_device, nullptr,
                               &mocked_user_interface_factory, &mocked_exiter);
      ui_test.run();

      THEN("make user_interface is called") {
        REQUIRE(mocked_user_interface_factory.make_called() == true);
      }

      THEN("gui loop is called") {
        REQUIRE(mocked_video_gui.loop_called() == true);
      }

      THEN("exit is called") { REQUIRE(mocked_exiter.exit_called() == true); }

      THEN("exit returns EXIT_SUCCESS") {
        REQUIRE(mocked_exiter.exit_value() == EXIT_SUCCESS);
      }
    }
    WHEN("one connected webcam") {
      std::vector<frank::video::input_device> one_device{
          frank::video::input_device()};
      test::frank::mock_exiter mocked_exiter{};
      test::frank::mock_video_gui mocked_video_gui{};
      auto mocked_video_gui_holder =
          std::make_unique<test::frank::mock_video_gui_holder>(
              mocked_video_gui);
      test::frank::mock_user_interface_factory mocked_user_interface_factory(
          mocked_video_gui_holder);

      frank::video::ui ui_test(one_device, nullptr,
                               &mocked_user_interface_factory, &mocked_exiter);
      ui_test.run();

      THEN("make user_interface is called") {
        REQUIRE(mocked_user_interface_factory.make_called() == true);
      }

      THEN("gui loop is called") {
        REQUIRE(mocked_video_gui.loop_called() == true);
      }

      THEN("exit is called") { REQUIRE(mocked_exiter.exit_called() == true); }

      THEN("exit returns EXIT_SUCCESS") {
        REQUIRE(mocked_exiter.exit_value() == EXIT_SUCCESS);
      }
    }
  }
}
