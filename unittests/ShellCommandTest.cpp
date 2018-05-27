
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../lib/Utils.h"

TEST_CASE("Escaping commands", "[ShellCommand]" ) {
  // Command only
  REQUIRE(Utils::buildShellCmd("a") == R"c("a")c");
  // Single arg
  REQUIRE(Utils::buildShellCmd("a", {"b"}) == R"c("a" "b")c");
  // Multiple args
  REQUIRE(Utils::buildShellCmd("a", {"b", "ba"}) == R"c("a" "b" "ba")c");
  // Quotes in argument
  REQUIRE(Utils::buildShellCmd("a", {"b", "b\"a"}) == R"c("a" "b" "b\"a")c");
}

TEST_CASE("Running commands", "[ShellCommand]" ) {
  // Command only
  REQUIRE(Utils::runCmd("echo", {"test"}) == "test\n");
}

TEST_CASE("Working dir", "[ShellCommand]" ) {
  // Command only
  REQUIRE(Utils::runCmd("echo", {"$PWD"}, "/tmp") == "/tmp\n");
}