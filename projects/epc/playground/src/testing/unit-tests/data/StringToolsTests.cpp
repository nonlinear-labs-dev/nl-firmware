#include <testing/TestHelper.h>
#include <tools/StringTools.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Splitting strings at spaces")
{
  REQUIRE(StringTools::splitStringAtSpacesAndTrimSpaces("Bar").size() == 1);
  REQUIRE(StringTools::splitStringAtSpacesAndTrimSpaces("Bar          Foo").size() == 2);
  REQUIRE(StringTools::splitStringAtSpacesAndTrimSpaces("BarFoo Bar Foo").size() == 3);
  REQUIRE(StringTools::splitStringAtSpacesAndTrimSpaces("  Foo Bar").size() == 2);
  REQUIRE(StringTools::splitStringAtSpacesAndTrimSpaces("  ").empty());
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"NL Stringtools")
{
  REQUIRE(nltools::string::removeCharacters("Foo", {}) == "Foo");
  REQUIRE(nltools::string::removeCharacters("Foo", {'o'}) == "F");
  REQUIRE(nltools::string::removeCharacters("Foof", {'F', 'f'}) == "oo");
  REQUIRE(nltools::string::removeCharacters("ABCabc", {'A', 'a'}) == "BCbc");
  REQUIRE(nltools::string::removeCharacters("ABCabc", {'A', 'a', 'B', 'b', 'C', 'c'}).empty());
}