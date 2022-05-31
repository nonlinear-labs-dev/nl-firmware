#include "testing/TestHelper.h"
#include "presets/Preset.h"
#include "presets/PresetManager.h"
#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <testing/unit-tests/mock/PreDualModePresetBank.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <proxies/audio-engine/AudioEngineProxy.h>

TEST_CASE("Preset Load sends EditBuffer")
{

  using namespace nltools::msg;
  using namespace std::chrono;

  Configuration configuration;
  configuration.useEndpoints = { EndPoint::AudioEngine };
  configuration.offerEndpoints = { EndPoint::Playground, EndPoint::AudioEngine };

  TestHelper::ScopedMessagingConfiguration scopeEndPoint { configuration };

  bool singleMessageRecieved = false;

  CHECK(waitForConnection(EndPoint::AudioEngine));
  auto c = receive<SinglePresetMessage>(EndPoint::AudioEngine,
                                        [&](const auto &singleEditMessage) { singleMessageRecieved = true; });

  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);

  MockPresetStorage presets;
  ebUseCases.load(presets.getSinglePreset());

  TestHelper::doMainLoop(1s, 1s, [&] { return singleMessageRecieved; });
  c.disconnect();

  CHECK(eb->getUUIDOfLastLoadedPreset() == presets.getSinglePreset()->getUuid());
}

TEST_CASE("Store Action does not send EditBuffer")
{
  using namespace nltools::msg;
  using namespace std::chrono;

  TestHelper::initSingleEditBuffer();

  Configuration configuration;
  configuration.useEndpoints = { EndPoint::AudioEngine };
  configuration.offerEndpoints = { EndPoint::Playground, EndPoint::AudioEngine };

  TestHelper::ScopedMessagingConfiguration scopeEndPoint { configuration };

  auto pm = TestHelper::getPresetManager();
  auto settings = TestHelper::getSettings();

  bool singleMessageReceived = false;


  CHECK(waitForConnection(EndPoint::AudioEngine));
  auto c = receive<SinglePresetMessage>(EndPoint::AudioEngine,
                                        [&](const auto &singleEditMessage) {
                                          singleMessageReceived = true;
                                        });
  PresetManagerUseCases useCases(*pm, *settings);

  //Store EditBuffer as new Bank
  auto oldNumBanks = pm->getNumBanks();
  useCases.createBankAndStoreEditBuffer();
  auto newNumBanks = pm->getNumBanks();
  TestHelper::doMainLoopIteration();
  CHECK(newNumBanks > oldNumBanks);
  CHECK(!singleMessageReceived);

  auto bank = pm->getSelectedBank();
  CHECK(bank != nullptr);
  BankUseCases bankUseCases(bank, *TestHelper::getSettings());

  //Append preset into bank
  auto oldNumPresets = bank->getNumPresets();
  const auto appendedPreset = bankUseCases.appendEditBuffer();
  auto newNumPresets = bank->getNumPresets();
  TestHelper::doMainLoopIteration();
  CHECK(newNumPresets > oldNumPresets);
  CHECK(!singleMessageReceived);

  //Insert preset into bank at pos 0
  oldNumPresets = bank->getNumPresets();
  auto insertedPreset = bankUseCases.insertEditBufferAtPosition(0);
  newNumPresets = bank->getNumPresets();
  TestHelper::doMainLoopIteration();
  CHECK(newNumPresets > oldNumPresets);
  CHECK(!singleMessageReceived);

  c.disconnect();
}