#include <testing/TestHelper.h>
#include <device-settings/Settings.h>
#include <device-settings/flac/FlacRecorderVirgin.h>
#include <sync/SyncMasterMockRoot.h>
#include <testing/unit-tests/mock/MockSettingsObject.h>
#include <filesystem>
#include <thread>
#include <nltools/GenericScopeGuard.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Issue 2900")
{
  using namespace std::chrono_literals;
  using namespace nltools::msg;

  auto oldConfig = nltools::msg::getConfig();
  oldConfig.useEndpoints.emplace_back(EndPoint::Playground);
  oldConfig.offerEndpoints.emplace_back(EndPoint::AudioEngine);
  REQUIRE(oldConfig.mainContext.get() != nullptr);

  init(oldConfig);

  auto file = "/tmp/playground-issue2900-settings.xml";
  std::filesystem::remove(file);
  auto settings = std::make_unique<MockSettingsObject>(file, &SyncMasterMockRoot::get());
  settings->init();

  WHEN("settings are created initial state is 'virgin'")
  {
    CHECK(settings->getSetting<FlacRecorderVirgin>()->get());
  }

  nltools::msg::waitForConnection(nltools::msg::EndPoint::Playground);

  WHEN("audioengine notifies state")
  {
    nltools::msg::send(nltools::msg::EndPoint::Playground, nltools::msg::Recorder::State { true });

    THEN("virgin setting becomes false")
    {
      for(int count = 0; settings->getSetting<FlacRecorderVirgin>()->get() && count < 100; count++)
        TestHelper::doMainLoopIteration();

      CHECK_FALSE(settings->getSetting<FlacRecorderVirgin>()->get());

      THEN("state is restored on next startup")
      {
        settings.reset();
        settings = std::make_unique<MockSettingsObject>(file, &SyncMasterMockRoot::get());
        settings->init();
        CHECK_FALSE(settings->getSetting<FlacRecorderVirgin>()->get());
      }
    }
  }
}