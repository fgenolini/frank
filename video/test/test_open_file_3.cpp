#include "config.h"

#ifndef _TEST_OPEN_FILE_3_
#error _TEST_OPEN_FILE_3_ not defined
#endif

WARNINGS_OFF
#include <iostream>

#include <catch2/catch.hpp>
WARNINGS_ON

#include "fake_file_dialogs.h"
#include "file/file_dialogs.h"

namespace test::frank {

WARNING_PUSH
DISABLE_WARNING_MSC(4820)
class open_file_mock {
public:
  open_file_mock(std::vector<std::string> const &fake_result)
      : fake_result_(fake_result) {}

  void construct() { constructor_called_ = true; }

  std::vector<std::string> result() {
    result_called_ = true;
    return fake_result_;
  }

  bool constructor_called() { return constructor_called_; }
  bool result_called() { return result_called_; }

private:
  std::vector<std::string> fake_result_;
  bool constructor_called_{};
  bool result_called_{};
};
WARNINGS_ON

} // namespace test::frank

// Faking portable-file-dialogs.h
namespace pfd {

open_file::open_file(std::string const &, std::string const &,
                     std::vector<std::string> const &, opt, void *mock_data)
    : mock_data_(mock_data) {
  auto mock = static_cast<::test::frank::open_file_mock *>(mock_data_);
  mock->construct();
}

std::vector<std::string> open_file::result() const {
  auto mock = static_cast<::test::frank::open_file_mock *>(mock_data_);
  return mock->result();
}

} // namespace pfd

SCENARIO("frank video open file 3", "[open_file_3]") {
  GIVEN("frank video GUI a file open dialog") {
    WHEN("no file") {
      constexpr auto RESULTS_SIZE = 0;
      constexpr auto TEST_PATH = "test path";
      constexpr auto TEST_TITLE = "test title";
      std::vector<std::string> const test_filters{"test filter"};
      std::vector<std::string> const no_result{};
      test::frank::open_file_mock mock{no_result};

      frank::video::file_dialogs dialogs{};
      auto results =
          dialogs.open_file(TEST_TITLE, TEST_PATH, test_filters, &mock);

      THEN("open_file is constructed") {
        REQUIRE(mock.constructor_called() == true);
      }

      THEN("result is called") { REQUIRE(mock.result_called() == true); }

      THEN("empty results are returned") {
        REQUIRE(results.size() == RESULTS_SIZE);
      }
    }

    WHEN("one file") {
      constexpr auto RESULTS_SIZE = 1;
      constexpr auto TEST_PATH = "test path";
      constexpr auto TEST_TITLE = "test title";
      std::vector<std::string> const test_filters{"test filter"};
      std::vector<std::string> const one_result{"test file"};
      test::frank::open_file_mock mock{one_result};

      frank::video::file_dialogs dialogs{};
      auto results =
          dialogs.open_file(TEST_TITLE, TEST_PATH, test_filters, &mock);

      THEN("open_file is constructed") {
        REQUIRE(mock.constructor_called() == true);
      }

      THEN("result is called") { REQUIRE(mock.result_called() == true); }

      THEN("results contain 1 element") {
        REQUIRE(results.size() == RESULTS_SIZE);
      }
    }
  }

  WHEN("several files") {
    constexpr auto RESULTS_SIZE = 2;
    constexpr auto TEST_PATH = "test path";
    constexpr auto TEST_TITLE = "test title";
    std::vector<std::string> const test_filters{"test filter"};
    std::vector<std::string> const several_results{"test file 1",
                                                   "test file 2"};
    test::frank::open_file_mock mock{several_results};

    frank::video::file_dialogs dialogs{};
    auto results =
        dialogs.open_file(TEST_TITLE, TEST_PATH, test_filters, &mock);

    THEN("open_file is constructed") {
      REQUIRE(mock.constructor_called() == true);
    }

    THEN("result is called") { REQUIRE(mock.result_called() == true); }

    THEN("results contain several elements") {
      REQUIRE(results.size() == RESULTS_SIZE);
    }
  }
}
