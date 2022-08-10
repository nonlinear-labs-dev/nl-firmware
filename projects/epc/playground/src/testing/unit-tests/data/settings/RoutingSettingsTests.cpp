#include <testing/TestHelper.h>
#include <device-settings/Settings.h>
#include <device-settings/midi/RoutingSettings.h>
#include <testing/unit-tests/mock/MockSettingsObject.h>
#include <sync/SyncMasterMockRoot.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"setAllValues will not notify if no values Changed")
{
  MockSettingsObject settings("", &SyncMasterMockRoot::get());

  auto setting = settings.getSetting<RoutingSettings>();

  const auto& data = setting->getRaw();

  WHEN("All Values are initialy true")
  {
    auto allValuesAreTrue = std::all_of(
        data.begin(), data.end(),
        [](const auto& row) { return std::all_of(row.begin(), row.end(), [](auto b) { return b == true; }); });

    CHECK(allValuesAreTrue);

    WHEN("All elements are set to current Value again")
    {
      setting->setAllValues(true);
      bool didNotifyArrive = false;
      auto connection = setting->onChange([&](auto) { didNotifyArrive = true; });

      setting->setAllValues(true);

      THEN("No notify was send")
      {
        CHECK_FALSE(didNotifyArrive);
      }
    }

    WHEN("all elements are changed")
    {
      bool didNotifyArrive = false;
      auto connection = setting->onChange([&](auto) { didNotifyArrive = true; });

      setting->setAllValues(false);

      THEN("notify was send")
      {
        CHECK(didNotifyArrive);
      }
    }

    WHEN("one elements is changed")
    {
      bool didNotifyArrive = false;
      auto connection = setting->onChange([&](auto) { didNotifyArrive = true; });

      setting->setState(RoutingSettings::tRoutingIndex::Notes, RoutingSettings::tAspectIndex::LOCAL, false);

      THEN("notify was send")
      {
        CHECK(didNotifyArrive);
      }
    }

    WHEN("on elements is set to current value again")
    {
      setting->setAllValues(true);
      bool didNotifyArrive = false;
      auto connection = setting->onChange([&](auto) { didNotifyArrive = true; });

      setting->setState(RoutingSettings::tRoutingIndex::Notes, RoutingSettings::tAspectIndex::LOCAL, true);

      THEN("notify was not send")
      {
        CHECK_FALSE(didNotifyArrive);
      }
    }
  }
}