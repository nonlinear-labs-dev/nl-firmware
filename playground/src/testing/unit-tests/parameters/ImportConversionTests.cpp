#include <testing/TestHelper.h>
#include <third-party/include/catch.hpp>
#include "parameters/ParameterImportConversions.h"

TEST_CASE("FB Drive 50db to 70db")
{
  auto& converter = ParameterImportConversions::get();
  auto twoSevenths = (2.0 / 7.0);
  REQUIRE(converter.convert(164, 0, 5) == twoSevenths);
  REQUIRE(converter.convert(164, 1, 5) == 1);
  REQUIRE(converter.convert(164, 0.5, 5) == 0.5 + twoSevenths);
}