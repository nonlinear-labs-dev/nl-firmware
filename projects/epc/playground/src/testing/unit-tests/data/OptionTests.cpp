#include <catch.hpp>
#include <device-settings/EditSmoothingTime.h>
#include <testing/TestHelper.h>
#include <device-settings/Settings.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Edit Smoothing Time - Scales")
{
  Settings settings("", nullptr);
  EditSmoothingTime time(settings);
  REQUIRE(time.getParent() != nullptr);
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"Edit Smoothing Time - To String")
{
  Settings settings("", nullptr);
  EditSmoothingTime time(settings);

  SECTION("0.000 ms")
  {
    time.set(0.0);
    REQUIRE(time.getDisplayString() == "0.000 ms");
  }
}