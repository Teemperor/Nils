
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../lib/Utils.h"

TEST_CASE("Escaping commands", "[ShellCommand]" ) {
  // Command only
  REQUIRE(Utils::buildShellCmd("a") == R"("a")");
  // Single arg
  REQUIRE(Utils::buildShellCmd("a", {"b"}) == R"("a" "b")");
  // Multiple args
  REQUIRE(Utils::buildShellCmd("a", {"b", "ba"}) == R"("a" "b" "ba")");
  // Quotes in argument
  REQUIRE(Utils::buildShellCmd("a", {"b", "b\"a"}) == R"("a" "b" "b\"a")");
}

TEST_CASE("Running commands", "[ShellCommand]" ) {
  REQUIRE(Utils::runCmd("echo", {"test"}).Stdout == "test\n");
  REQUIRE(Utils::runCmd("echo", {"test"}).ExitCode == 0);

  REQUIRE(Utils::runCmd("mkdir", {"/not/allowed"}).Stdout != "");
  REQUIRE(Utils::runCmd("mkdir", {"/not/allowed"}).ExitCode != 0);
}

TEST_CASE("Working dir", "[ShellCommand]" ) {
  REQUIRE(Utils::runCmd("echo", {"$PWD"}, "/tmp").Stdout == "/tmp\n");
  REQUIRE(Utils::runCmd("echo", {"$PWD"}, "/tmp").ExitCode == 0);
}