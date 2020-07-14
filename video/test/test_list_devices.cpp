#include "config.h"

#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include "../list_devices.h"

SCENARIO("frank video list devices", "[list_video_devices]") {
  GIVEN("list_input_devices called on a computer capable of video input") {
    WHEN("no video input device is connected to the computer") {
      std::vector<std::string> no_device{};

      REQUIRE(no_device.size() == 0);

      auto results = frank::video::list_input_devices(&no_device);

      THEN("an empty list of devices is returned") {
        REQUIRE(results.size() == 0);
      }
    }
    WHEN("One video input device is connected to the computer") {
      std::vector<std::string> one_device{"test device 0"};

      REQUIRE(one_device.size() == 1);

      auto results = frank::video::list_input_devices(&one_device);

      THEN("a list of 1 devices is returned") { REQUIRE(results.size() == 1); }
    }
    WHEN("Five video input devices are connected to the computer") {
      std::vector<std::string> five_devices{"test device 0", "test device 1",
                                            "test device 2", "test device 3",
                                            "test device 4"};

      REQUIRE(five_devices.size() == 5);

      auto results = frank::video::list_input_devices(&five_devices);

      THEN("a list of 5 devices is returned") { REQUIRE(results.size() == 5); }
    }
  }
}
