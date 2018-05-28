
#define CATCH_CONFIG_MAIN
#include "Utils.h"
#include "catch.hpp"

TEST_CASE("Splitting strings", "[Utils]") {
  std::vector<std::string> Result;
  splitStr("A", ':', Result);
  REQUIRE(Result == std::vector<std::string>({"A"}));
}