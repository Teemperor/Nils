
#define CATCH_CONFIG_MAIN
#include "NativePasses/PassUtils.h"
#include "catch.hpp"

TEST_CASE("Tokenize", "[PassUtils]") {
  REQUIRE(PassUtils::tokenize("A").front().Content == "A");
}