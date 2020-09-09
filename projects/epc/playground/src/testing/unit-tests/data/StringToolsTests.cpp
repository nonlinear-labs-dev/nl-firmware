#include <testing/TestHelper.h>
#include <tools/StringTools.h>

TEST_CASE("Splitting strings at spaces")
{
  REQUIRE(StringTools::splitStringAtSpacesAndTrimSpaces("Bar").size() == 1);
  REQUIRE(StringTools::splitStringAtSpacesAndTrimSpaces("Bar          Foo").size() == 2);
  REQUIRE(StringTools::splitStringAtSpacesAndTrimSpaces("BarFoo Bar Foo").size() == 3);
  REQUIRE(StringTools::splitStringAtSpacesAndTrimSpaces("  Foo Bar").size() == 2);
  REQUIRE(StringTools::splitStringAtSpacesAndTrimSpaces("  ").size() == 0);
}
