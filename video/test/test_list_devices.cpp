#include "config.h"

#include <string>
#include <vector>

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

#include "device/device_register.h"
#include "device/list_devices.h"

namespace test::frank {

class mock_device_register
    : public trompeloeil::mock_interface<::frank::video::device_register> {
public:
  IMPLEMENT_CONST_MOCK0(name_devices);
};

} // namespace test::frank

SCENARIO("frank video list devices", "[list_video_devices]") {
  GIVEN("list_input_devices called on a computer capable of video input") {
    WHEN("no video input device is connected to the computer") {
      std::vector<std::string> no_device{};
      test::frank::mock_device_register mock{};
      REQUIRE_CALL(mock, name_devices()).LR_RETURN(no_device);

      REQUIRE(no_device.size() == 0);

      auto results = frank::video::list_input_devices(&mock);

      THEN("an empty list of devices is returned") {
        REQUIRE(results.size() == 0);
      }
    }
    WHEN("One video input device is connected to the computer") {
      std::vector<std::string> one_device{"test device 0"};
      test::frank::mock_device_register mock{};
      REQUIRE_CALL(mock, name_devices()).LR_RETURN(one_device);

      REQUIRE(one_device.size() == 1);

      auto results = frank::video::list_input_devices(&mock);

      THEN("a list of 1 devices is returned") { REQUIRE(results.size() == 1); }
    }
    WHEN("Five video input devices are connected to the computer") {
      std::vector<std::string> five_devices{"test device 0", "test device 1",
                                            "test device 2", "test device 3",
                                            "test device 4"};
      test::frank::mock_device_register mock{};
      REQUIRE_CALL(mock, name_devices()).LR_RETURN(five_devices);

      REQUIRE(five_devices.size() == 5);

      auto results = frank::video::list_input_devices(&mock);

      THEN("a list of 5 devices is returned") { REQUIRE(results.size() == 5); }
    }
  }
}
