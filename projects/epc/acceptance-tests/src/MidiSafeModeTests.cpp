#include <catch.hpp>
#include <mock/XMLPresetLoader.h>
#include <memory>
#include <synth/C15Synth.h>
#include <AudioEngineOptions.h>
#include <Toolbox.h>
#include <testing/TestHelper.h>
#include <mock/MockSettingsObject.h>
#include <sync/SyncMasterMockRoot.h>

TEST_CASE("Midi Safe Mode disabled")
{
  // Prepare Configuration
  auto config = nltools::msg::getConfig();
  config.useEndpoints
      = { nltools::msg::EndPoint::Playground, nltools::msg::EndPoint::AudioEngine, nltools::msg::EndPoint::BeagleBone };
  config.offerEndpoints
      = { nltools::msg::EndPoint::Playground, nltools::msg::EndPoint::AudioEngine, nltools::msg::EndPoint::BeagleBone };
  nltools::msg::init(config);
  auto options = Tests::createEmptyAudioEngineOptions();
  auto synth = std::make_unique<C15Synth>(options.get());
  MockSettingsObject settings(&SyncMasterMockRoot::get());

  // Prepare Midi Settings
  using tMSG = nltools::msg::Setting::MidiSettingsMessage;
  tMSG msg;
  TestHelper::updateMappingForHW(msg.routings, tMSG::RoutingIndex::Notes, tMSG::RoutingAspect::LOCAL, true);
  msg.localEnable = true;
  msg.safeMode = false;
  synth->onMidiSettingsMessage(msg);

  // Prepare Preset
  auto settingBasePtr = static_cast<Settings*>(&settings);
  // fails (SIGSEGV):
  XMLPresetLoader::loadTestPresetFromBank(synth.get(), "xml-banks", "SplitPlateau", *settingBasePtr);
  // useless preset:
  // XMLPresetLoader::loadTestPresetFromBank(synth.get(), "xml-banks", "InitWithAMix", *settingBasePtr);

  // play TCD Notes:
  synth->doTcd({ 0xED, 0, 48 });     // keyPos 48 (C2)
  synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)
  synth->doTcd({ 0xED, 0, 52 });     // keyPos 52 (E2)
  synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)
  synth->doTcd({ 0xED, 0, 55 });     // keyPos 55 (G2)
  synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)

  // get level for later comparison
  const auto resultingLevel
      = Tests::getMaxLevel(std::get<0>(synth->measurePerformance(std::chrono::milliseconds(250))));

  // release TCD Notes:
  synth->doTcd({ 0xED, 0, 48 });     // keyPos 48 (C2)
  synth->doTcd({ 0xEF, 127, 127 });  // keyUp (100% Velocity)
  synth->doTcd({ 0xED, 0, 52 });     // keyPos 52 (E2)
  synth->doTcd({ 0xEF, 127, 127 });  // keyUp (100% Velocity)
  synth->doTcd({ 0xED, 0, 55 });     // keyPos 55 (G2)
  synth->doTcd({ 0xEF, 127, 127 });  // keyUp (100% Velocity)

  synth->measurePerformance(std::chrono::milliseconds(250));

  // Prepare Settings
  msg.localEnable = false;
  synth->onMidiSettingsMessage(msg);

  // play TCD Notes:
  synth->doTcd({ 0xED, 0, 48 });     // keyPos 48 (C2)
  synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)
  synth->doTcd({ 0xED, 0, 52 });     // keyPos 52 (E2)
  synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)
  synth->doTcd({ 0xED, 0, 55 });     // keyPos 55 (G2)
  synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)

  WHEN("multiple Notes played with Split Sound and Local disabled")
  {
    auto res = synth->measurePerformance(std::chrono::milliseconds(250));
    THEN("produces no Sound")
    {
      CHECK(Tests::getMaxLevel(std::get<0>(res)) == 0);
    }
  }

  WHEN("multiple Notes played/held with Split Sound and Local re-enabled")
  {
    synth->measurePerformance(std::chrono::milliseconds(250));

    // update settings (Notes still active)
    msg.localEnable = true;
    synth->onMidiSettingsMessage(msg);

    // release TCD Notes:
    synth->doTcd({ 0xED, 0, 48 });     // keyPos 48 (C2)
    synth->doTcd({ 0xEF, 127, 127 });  // keyUp (100% Velocity)
    synth->doTcd({ 0xED, 0, 52 });     // keyPos 52 (E2)
    synth->doTcd({ 0xEF, 127, 127 });  // keyUp (100% Velocity)
    synth->doTcd({ 0xED, 0, 55 });     // keyPos 55 (G2)
    synth->doTcd({ 0xEF, 127, 127 });  // keyUp (100% Velocity)

    synth->measurePerformance(std::chrono::milliseconds(250));

    // play TCD Notes (again):
    synth->doTcd({ 0xED, 0, 48 });     // keyPos 48 (C2)
    synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)
    synth->doTcd({ 0xED, 0, 52 });     // keyPos 52 (E2)
    synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)
    synth->doTcd({ 0xED, 0, 55 });     // keyPos 55 (G2)
    synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)

    THEN("multiple Notes played again produces multiple active Voices")
    {
      auto res = synth->measurePerformance(std::chrono::milliseconds(250));
      CHECK(Tests::getMaxLevel(std::get<0>(res)) == resultingLevel);
    }
  }
}
