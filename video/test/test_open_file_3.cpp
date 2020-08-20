#include "config.h"

#ifndef _TEST_OPEN_FILE_3_
#error _TEST_OPEN_FILE_3_ not defined
#endif

WARNINGS_OFF
#include <catch2/catch.hpp>
WARNINGS_ON

#include "fake_file_dialogs.h"
#include "file/file_dialogs.h"

namespace test::frank {} // namespace test::frank

// Faking portable-file-dialogs.h
namespace pfd {

open_file::open_file(std::string const &, std::string const &,
                     std::vector<std::string> const &, opt) {
  execute_called = true;
}

std::vector<std::string> open_file::result() {
  result_called = true;
  return fake_result;
}

bool open_file::execute_called{};
bool open_file::result_called{};
std::vector<std::string> open_file::fake_result{};

} // namespace pfd

SCENARIO("frank video open file 3", "[open_file_3]") {
  GIVEN("frank video GUI a file open dialog") {
    WHEN("no file") {
      constexpr auto RESULTS_SIZE = 0;
      constexpr auto TEST_PATH = "test path";
      constexpr auto TEST_TITLE = "test title";
      std::vector<std::string> const test_filters{"test filter"};
      std::vector<std::string> const no_result{};
      pfd::open_file::execute_called = false;
      pfd::open_file::result_called = false;
      pfd::open_file::fake_result = no_result;

      frank::video::file_dialogs dialogs{};
      auto results = dialogs.open_file(TEST_TITLE, TEST_PATH, test_filters);

      THEN("open_file is constructed") {
        REQUIRE(pfd::open_file::execute_called == true);
      }

      THEN("result is called") {
        REQUIRE(pfd::open_file::result_called == true);
      }

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
      pfd::open_file::execute_called = false;
      pfd::open_file::result_called = false;
      pfd::open_file::fake_result = one_result;

      frank::video::file_dialogs dialogs{};
      auto results = dialogs.open_file(TEST_TITLE, TEST_PATH, test_filters);

      THEN("open_file is constructed") {
        REQUIRE(pfd::open_file::execute_called == true);
      }

      THEN("result is called") {
        REQUIRE(pfd::open_file::result_called == true);
      }

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
    std::vector<std::string> const several_results{"test file 1", "test file 2",
                                                   ""};
    pfd::open_file::execute_called = false;
    pfd::open_file::result_called = false;
    pfd::open_file::fake_result = several_results;

    frank::video::file_dialogs dialogs{};
    auto results = dialogs.open_file(TEST_TITLE, TEST_PATH, test_filters);

    THEN("open_file is constructed") {
      REQUIRE(pfd::open_file::execute_called == true);
    }

    THEN("result is called") { REQUIRE(pfd::open_file::result_called == true); }

    THEN("results contain several elements") {
      REQUIRE(results.size() == RESULTS_SIZE);
    }
  }
}
