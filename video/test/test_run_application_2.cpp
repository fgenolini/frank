#include "config.h"

#ifndef _TEST_RUN_APPLICATION_2_
#error _TEST_RUN_APPLICATION_2_ not defined
#endif

WARNINGS_OFF
#include <exception>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "application.h"
#include "device/video_devices.h"
#include "exception/exceptions.h"
#include "ui/ui.h"

namespace test::frank {

struct run_application_exception : public std::exception {
  char const *what() const noexcept override {
    return "run_application_2 exception";
  }
};

enum class exception_type {
  none,
  exception_object,
  int_number,
};

constexpr auto INT_EXCEPTION = 42;

class mock_exceptions {
public:
  void handler(std::exception const *caught_exception = nullptr) noexcept {
    handler_called_ = true;
    caught_exception_ = caught_exception;
  }

  bool handler_called() { return handler_called_; }

private:
  std::exception const *caught_exception_{};
  bool handler_called_{};
};

class mock_ui {
public:
  void run() { run_called_ = true; }

  bool run_called() { return run_called_; }

private:
  bool run_called_{};
};

class mock_video_devices : public ::frank::video::video_devices {
public:
  mock_video_devices(
      exception_type throw_in_list_devices = exception_type::none)
      : ::frank::video::video_devices(nullptr),
        throw_in_list_devices_(throw_in_list_devices) {}

  ~mock_video_devices() override {}

  std::vector<::frank::video::input_device> list() override {
    list_devices_called_ = true;
    if (throw_in_list_devices_ == exception_type::exception_object)
      throw exception_;

    if (throw_in_list_devices_ == exception_type::int_number)
      throw INT_EXCEPTION;

    return std::vector<::frank::video::input_device>();
  }

  bool list_devices_called() { return list_devices_called_; }

private:
  exception_type throw_in_list_devices_{};
  run_application_exception exception_{};
  bool list_devices_called_{};
};

} // namespace test::frank

namespace frank::video {

cvui_init::~cvui_init() {}
void cvui_init::execute(const std::string[], size_t) {}

exceptions::exceptions(aborter *, void *mock_data)
    : aborter_(nullptr), mock_data_(mock_data) {}

void exceptions::handler(std::exception const *caught_exception) noexcept {
  if (aborter_) {
  }

  auto mock = static_cast<::test::frank::mock_exceptions *>(mock_data_);
  mock->handler(caught_exception);
}

static std::vector<input_device> dummy_empty{};

ui::ui(std::vector<input_device> const &, cvui_init *, user_interface_factory *,
       exiter *, void *mock_data)
    : initialise_windows_(nullptr), exiter_(nullptr),
      connected_webcams_(dummy_empty), make_ui_(nullptr),
      mock_data_(mock_data) {}

void ui::run() {
  auto mock = static_cast<::test::frank::mock_ui *>(mock_data_);
  mock->run();
}

video_devices::video_devices(standard_io *) : stdio_(nullptr) {}

video_devices::~video_devices() {}

std::vector<input_device> video_devices::list() {
  if (stdio_) {
  }

  return std::vector<input_device>();
}

void unhandled_exception_handler() noexcept {}

} // namespace frank::video

SCENARIO("frank video run application 2", "[run_application_2]") {
  GIVEN("run the frank video application") {
    WHEN("no argument") {
      test::frank::mock_video_devices mocked_video_devices{};
      test::frank::mock_ui mocked_ui{};
      frank::video::ui ui(std::vector<frank::video::input_device>(), nullptr,
                          nullptr, nullptr, &mocked_ui);
      test::frank::mock_exceptions mocked_exceptions{};
      frank::video::exceptions exceptions(nullptr, &mocked_exceptions);

      frank::video::application app(&mocked_video_devices, &ui, &exceptions);
      app.run();

      THEN("list_devices is called") {
        REQUIRE(mocked_video_devices.list_devices_called() == true);
      }

      THEN("run_ui is called") { REQUIRE(mocked_ui.run_called() == true); }

      THEN("exceptions_handler is not called") {
        REQUIRE(mocked_exceptions.handler_called() == false);
      }
    }
    WHEN("exception object thrown in list_devices") {
      constexpr auto THROW_IN_LIST_DEVICES =
          test::frank::exception_type::exception_object;
      test::frank::mock_video_devices mocked_video_devices{
          THROW_IN_LIST_DEVICES};
      test::frank::mock_ui mocked_ui{};
      frank::video::ui ui(std::vector<frank::video::input_device>(), nullptr,
                          nullptr, nullptr, &mocked_ui);
      test::frank::mock_exceptions mocked_exceptions{};
      frank::video::exceptions exceptions(nullptr, &mocked_exceptions);

      frank::video::application app(&mocked_video_devices, &ui, &exceptions);
      app.run();

      THEN("list_devices is called") {
        REQUIRE(mocked_video_devices.list_devices_called() == true);
      }

      THEN("run_ui is not called") { REQUIRE(mocked_ui.run_called() == false); }

      THEN("exceptions_handler is called") {
        REQUIRE(mocked_exceptions.handler_called() == true);
      }
    }
    WHEN("int number thrown in list_devices") {
      constexpr auto THROW_IN_LIST_DEVICES =
          test::frank::exception_type::int_number;
      test::frank::mock_video_devices mocked_video_devices{
          THROW_IN_LIST_DEVICES};
      test::frank::mock_ui mocked_ui{};
      frank::video::ui ui(std::vector<frank::video::input_device>(), nullptr,
                          nullptr, nullptr, &mocked_ui);
      test::frank::mock_exceptions mocked_exceptions{};
      frank::video::exceptions exceptions(nullptr, &mocked_exceptions);

      frank::video::application app(&mocked_video_devices, &ui, &exceptions);
      app.run();

      THEN("list_devices is called") {
        REQUIRE(mocked_video_devices.list_devices_called() == true);
      }

      THEN("run_ui is not called") { REQUIRE(mocked_ui.run_called() == false); }

      THEN("exceptions_handler is called") {
        REQUIRE(mocked_exceptions.handler_called() == true);
      }
    }
  }
}
