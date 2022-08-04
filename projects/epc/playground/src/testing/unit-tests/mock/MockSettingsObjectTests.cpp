#include <testing/TestHelper.h>
#include <device-settings/Settings.h>
#include <device-settings/midi/RoutingSettings.h>
#include <testing/unit-tests/mock/MockSettingsObject.h>
#include <sync/SyncMasterMockRoot.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"MockSettingsObject onChange is called when Setting calls notify")
{
  MockSettingsObject settings("", &SyncMasterMockRoot::get());
  auto setting = settings.getSetting<RoutingSettings>();

  bool parentSubRecChanges = false;
  bool childPubsChanges = false;
  settings.setOnChangeCB([&](auto) {
    parentSubRecChanges = true;
    return 0;
  });

  auto connection = setting->onChange([&](const Setting* s) { childPubsChanges = true; });

  setting->setState(RoutingSettings::tRoutingIndex::Notes, RoutingSettings::tAspectIndex::LOCAL, false);

  CHECK(parentSubRecChanges == true);
  CHECK(childPubsChanges == true);
}