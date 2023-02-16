#include <catch.hpp>
#include <device-settings/EditSmoothingTime.h>
#include <testing/TestHelper.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/HardwareFeatures.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Edit Smoothing Time - Scales")
{
  HardwareFeatures hw;
  Settings settings("", nullptr, hw);
  EditSmoothingTime time(settings);
  REQUIRE(time.getParent() != nullptr);
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Edit Smoothing Time - To String")
{
  HardwareFeatures hw;
  Settings settings("", nullptr, hw);
  EditSmoothingTime time(settings);

  SECTION("0.000 ms")
  {
    time.set(0.0);
    REQUIRE(time.getDisplayString() == "0.000 ms");
  }
}