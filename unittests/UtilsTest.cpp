
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Utils.h"

TEST_CASE("Splitting strings", "[Utils]" ) {
  std::vector<std::string> Result;
  splitStr("A", ':', Result);
  REQUIRE(Result == std::vector<std::string>({"A"}));
}