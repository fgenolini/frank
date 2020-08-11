#include "config.h"

WARNINGS_OFF
#include <string>
#include <vector>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "device/list_devices.h"
#include "test/do_nothing_cstdio.h"
#include "test/testable_cstdio.h"

namespace test::frank {

static FILE *const TEST_FILE = (FILE *)0x12345678;

constexpr auto END_OF_FILE = 1;

WARNINGS_OFF
class mock_list_devices {
public:
  mock_list_devices(std::vector<std::string> const &test_devices)
      : test_devices_(test_devices) {}

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
};
WARNINGS_ON

int do_nothing_feof(FILE *file, void *mock_data) {
  auto mock = static_cast<::test::frank::mock_list_devices *>(mock_data);
  return mock->feof(file);
}

char *do_nothing_fgets(char *buffer, int buffer_size, FILE *file,
                       void *mock_data) {
  auto mock = static_cast<::test::frank::mock_list_devices *>(mock_data);
  return mock->fgets(buffer, buffer_size, file);
}

void do_nothing_pclose(FILE *file, void *mock_data) {
  auto mock = static_cast<::test::frank::mock_list_devices *>(mock_data);
  mock->pclose(file);
}

FILE *do_nothing_popen(char const *, char const *, void *mock_data) {
  auto mock = static_cast<::test::frank::mock_list_devices *>(mock_data);
  return mock->popen();
}

} // namespace test::frank

SCENARIO("frank video list devices", "[list_devices]") {
  GIVEN("list_devices called on a computer capable of video input") {
    WHEN("no video input device is connected to the computer") {
      constexpr auto DEVICE_COUNT = 0;
      std::vector<std::string> no_device{};
      test::frank::mock_list_devices mock{no_device};

      REQUIRE(no_device.size() == DEVICE_COUNT);

      auto results = frank::video::list_devices(&mock);

      THEN("an empty list of devices is returned") {
        REQUIRE(results.size() == DEVICE_COUNT);
      }
    }
    WHEN("One video input device is connected to the computer") {
      constexpr auto DEVICE_COUNT = 1;
      std::vector<std::string> one_device{"test device 0"};
      test::frank::mock_list_devices mock{one_device};

      REQUIRE(one_device.size() == DEVICE_COUNT);

      auto results = frank::video::list_devices(&mock);

      THEN("a list of 1 devices is returned") {
        REQUIRE(results.size() == DEVICE_COUNT);
      }
    }
    WHEN("Five video input devices are connected to the computer") {
      constexpr auto DEVICE_COUNT = 5;
      std::vector<std::string> five_devices{"test device 0", "test device 1",
                                            "test device 2", "test device 3",
                                            "test device 4"};
      test::frank::mock_list_devices mock{five_devices};

      REQUIRE(five_devices.size() == DEVICE_COUNT);

      auto results = frank::video::list_devices(&mock);

      THEN("a list of 5 devices is returned") {
        REQUIRE(results.size() == DEVICE_COUNT);
      }
    }
  }
}
