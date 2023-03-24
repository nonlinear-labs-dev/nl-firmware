#include "testing/TestHelper.h"
#include "presets/Preset.h"
#include "presets/PresetManager.h"
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <testing/unit-tests/mock/PreDualModePresetBank.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <proxies/audio-engine/AudioEngineProxy.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Preset Load sends EditBuffer")
{
  using namespace nltools::msg;
  using namespace std::chrono;

  auto oldConfig = nltools::msg::getConfig();

  oldConfig.useEndpoints.insert(EndPoint::AudioEngine);
  oldConfig.offerEndpoints.insert(EndPoint::AudioEngine);

  TestHelper::ScopedMessagingConfiguration scopeEndPoint { oldConfig };

  bool singleMessageRecieved = false;

  CHECK(waitForConnection(EndPoint::AudioEngine));
  auto c = receive<nltools::msg::SinglePresetMessage>(
      EndPoint::AudioEngine, [&](const auto &singleEditMessage) { singleMessageRecieved = true; });

  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);

  MockPresetStorage presets;
  ebUseCases.load(presets.getSinglePreset());

  TestHelper::doMainLoop(1ms, 1s, [&] { return singleMessageRecieved; });
  c.disconnect();

  CHECK(eb->getUUIDOfLastLoadedPreset() == presets.getSinglePreset()->getUuid());
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Store Action does not send EditBuffer")
{
  using namespace nltools::msg;
  using namespace std::chrono;

  TestHelper::initSingleEditBuffer();

  Configuration configuration;
  configuration.useEndpoints = { EndPoint::AudioEngine };
  configuration.offerEndpoints = { EndPoint::AudioEngine };

  TestHelper::ScopedMessagingConfiguration scopeEndPoint { configuration };

  auto pm = TestHelper::getPresetManager();
  auto settings = TestHelper::getSettings();

  bool singleMessageReceived = false;

  CHECK(waitForConnection(EndPoint::AudioEngine, std::chrono::seconds(1000)));
  auto c = receive<nltools::msg::SinglePresetMessage>(
      EndPoint::AudioEngine, [&](const auto &singleEditMessage) { singleMessageReceived = true; });
  PresetManagerUseCases useCases(*pm, *settings);

  //Store EditBuffer as new Bank
  auto oldNumBanks = pm->getNumBanks();
  useCases.createBankAndStoreEditBuffer();
  auto newNumBanks = pm->getNumBanks();
  TestHelper::doMainLoopIteration();
  CHECK(newNumBanks > oldNumBanks);
  TestHelper::doMainLoop(1ms, 1s, [&] { return !singleMessageReceived; });

  auto bank = pm->getSelectedBank();
  CHECK(bank != nullptr);
  BankUseCases bankUseCases(bank, *TestHelper::getSettings());

  //Append preset into bank
  auto oldNumPresets = bank->getNumPresets();
  const auto appendedPreset = bankUseCases.appendEditBuffer();
  auto newNumPresets = bank->getNumPresets();
  TestHelper::doMainLoopIteration();
  CHECK(newNumPresets > oldNumPresets);
  TestHelper::doMainLoop(1ms, 1s, [&] { return !singleMessageReceived; });

  //Insert preset into bank at pos 0
  oldNumPresets = bank->getNumPresets();
  auto insertedPreset = bankUseCases.insertEditBufferAtPosition(0);
  newNumPresets = bank->getNumPresets();
  TestHelper::doMainLoopIteration();
  CHECK(newNumPresets > oldNumPresets);
  TestHelper::doMainLoop(1ms, 1s, [&] { return !singleMessageReceived; });

  c.disconnect();
}