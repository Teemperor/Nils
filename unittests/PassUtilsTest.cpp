
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "NativePasses/PassUtils.h"

TEST_CASE("Tokenize", "[PassUtils]" ) {
  REQUIRE(PassUtils::tokenize("A").front().Content == "A");
}