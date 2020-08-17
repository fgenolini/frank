#include "config.h"

WARNINGS_OFF
#include <string>
#include <vector>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "device/video_devices.h"
#include "test/testable_cstdio.h"

namespace test::frank {

static FILE *const TEST_FILE = (FILE *)0x12345678;

constexpr auto END_OF_FILE = 1;

WARNINGS_OFF
class mock_list_devices {
public:
  mock_list_devices(std::vector<std::string> const &test_devices,
                    bool popen_fails = false)
      : test_devices_(test_devices), popen_fails_(popen_fails) {}

  int feof(FILE *file) {
    ++feof_count_;
    if (file != file_)
      return END_OF_FILE;

#if defined(APPLE) && defined(UNIX) && !defined(WIN32)
    return fgets_count > test_devices_.size() ? END_OF_FILE : 0;
#else
    return END_OF_FILE;
#endif
  }

  char *fgets(char *buffer, int buffer_size, FILE *file) {
    ++fgets_count;
    if (file != file_ || !buffer || buffer_size < 1)
      return nullptr;

#if defined(APPLE) && defined(UNIX) && !defined(WIN32)
    if (fgets_count > test_devices_.size())
      return nullptr;

    std::string test_buffer = test_devices_[fgets_count - 1];
    test_buffer += "\n";
    char const *text = test_buffer.c_str();
    for (auto i = 0; i < test_buffer.size() && i < buffer_size; ++i) {
      buffer[i] = text[i];
    }

    if (test_buffer.size() < buffer_size)
      buffer[test_buffer.size()] = 0;

    return buffer;
#else
    return nullptr;
#endif
  }

  void pclose(FILE *file) {
    pclose_called = true;
    if (file != file_)
      return;

    file_ = nullptr;
  }

  FILE *popen() {
    popen_called = true;
    if (popen_fails_)
      return nullptr;

#if defined(APPLE) && defined(UNIX) && !defined(WIN32)
    file_ = TEST_FILE;
    return TEST_FILE;
#else
    return nullptr;
#endif
  }

  bool feof_called() { return feof_count_ > 0; }
  bool fgets_called() { return fgets_count > 0; }
  bool pclose_called{};
  bool popen_called{};

private:
  std::vector<std::string> test_devices_;
  FILE *file_{};
  int feof_count_{};
  int fgets_count{};
  bool popen_fails_;
};
WARNINGS_ON

class mock_standard_io : public ::frank::video::standard_io {
public:
  mock_standard_io(mock_list_devices &mock) : mock_(mock) {}

  int feof(FILE *file) override { return mock_.feof(file); }

  char *fgets(char *buffer, int buffer_size, FILE *file) override {
    return mock_.fgets(buffer, buffer_size, file);
  }

  void pclose(FILE *file) override { mock_.pclose(file); }

  FILE *popen(char const *, char const *) override { return mock_.popen(); }

private:
  mock_list_devices &mock_;
};

} // namespace test::frank

SCENARIO("frank video list devices", "[list_devices]") {
  GIVEN("list_devices called on a computer capable of video input") {
    WHEN("no video input device is connected to the computer") {
      constexpr auto DEVICE_COUNT = 0;
      std::vector<std::string> no_device{};
      test::frank::mock_list_devices mock{no_device};
      test::frank::mock_standard_io io{mock};

      REQUIRE(no_device.size() == DEVICE_COUNT);

      frank::video::video_devices devices{&io};
      auto results = devices.list();

      THEN("an empty list of devices is returned") {
#if defined(APPLE) && defined(UNIX)
        REQUIRE(results.size() == DEVICE_COUNT);
#endif
      }
    }
    WHEN("One video input device is connected to the computer") {
      constexpr auto DEVICE_COUNT = 1;
      std::vector<std::string> one_device{"test device 0"};
      test::frank::mock_list_devices mock{one_device};
      test::frank::mock_standard_io io{mock};

      REQUIRE(one_device.size() == DEVICE_COUNT);

      frank::video::video_devices devices{&io};
      auto results = devices.list();

      THEN("a list of 1 devices is returned") {
#if defined(APPLE) && defined(UNIX)
        REQUIRE(results.size() == DEVICE_COUNT);
#endif
      }
    }
    WHEN("Five video input devices are connected to the computer") {
      constexpr auto DEVICE_COUNT = 5;
      std::vector<std::string> five_devices{"test device 0", "test device 1",
                                            "test device 2", "test device 3",
                                            "test device 4"};
      test::frank::mock_list_devices mock{five_devices};
      test::frank::mock_standard_io io{mock};

      REQUIRE(five_devices.size() == DEVICE_COUNT);

      frank::video::video_devices devices{&io};
      auto results = devices.list();

      THEN("a list of 5 devices is returned") {
#if defined(APPLE) && defined(UNIX)
        REQUIRE(results.size() == DEVICE_COUNT);
#endif
      }
#if defined(APPLE) && defined(UNIX)
      WHEN("Apple Mac OSX script could not be opened") {
        constexpr auto DEVICE_COUNT = 0;
        constexpr auto POPEN_FAILS = true;
        std::vector<std::string> five_devices{"test device 0", "test device 1",
                                              "test device 2", "test device 3",
                                              "test device 4"};
        test::frank::mock_list_devices mock(five_devices, POPEN_FAILS);
        test::frank::mock_standard_io io{mock};

        REQUIRE(five_devices.size() == 5);

        frank::video::video_devices devices{&io};
        auto results = devices.list();

        THEN("an empty list of devices is returned") {
          REQUIRE(results.size() == DEVICE_COUNT);
        }
      }
#endif
    }
  }
}
