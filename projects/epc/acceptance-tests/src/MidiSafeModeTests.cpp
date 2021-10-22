#include <catch.hpp>
#include <mock/XMLPresetLoader.h>
#include <memory>
#include <synth/C15Synth.h>
#include <AudioEngineOptions.h>
#include <Toolbox.h>
#include <testing/TestHelper.h>
#include <mock/MockSettingsObject.h>
#include <sync/SyncMasterMockRoot.h>

TEST_CASE("Midi Safe Mode Disabled")
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

  //Prepare Midi Settings
  using tMSG = nltools::msg::Setting::MidiSettingsMessage;
  tMSG msg;
  TestHelper::updateMappingForHW(msg.routings, tMSG::RoutingIndex::Notes, tMSG::RoutingAspect::LOCAL, true);
  msg.localEnable = false;
  msg.safeMode = false;
  synth->onMidiSettingsMessage(msg);

  // Prepare Preset
  auto settingBasePtr = static_cast<Settings*>(&settings);
  // fails (SIGSEGV):
  //    XMLPresetLoader::loadTestPresetFromBank(synth.get(), "xml-banks", "SplitPlateau", *settingBasePtr);
  // useless preset:
  XMLPresetLoader::loadTestPresetFromBank(synth.get(), "xml-banks", "InitWithAMix", *settingBasePtr);

  // TCD Notes:
  synth->doTcd({ 0xED, 0, 48 });     // keyPos 48 (C2)
  synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)
  synth->doTcd({ 0xED, 0, 52 });     // keyPos 48 (E2)
  synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)
  synth->doTcd({ 0xED, 0, 55 });     // keyPos 48 (G2)
  synth->doTcd({ 0xEE, 127, 127 });  // keyDown (100% Velocity)

  WHEN("Split Sound with Local Disabled")
  {
    auto res = synth->measurePerformance(std::chrono::milliseconds(250));
    THEN("Note Played produces no Sound")
    {
      CHECK(Tests::getMaxLevel(std::get<0>(res)) == 0);
    }
  }

  //  WHEN("Init Preset is Loaded")
  //  {
  //    XMLPresetLoader::loadTestPresetFromBank(synth.get(), "xml-banks", "Init", *settingBasePtr);
  //    THEN("Note Played produces no Sound")
  //    {
  //      synth->doMidi({ 0x90, 127, 127 });
  //      auto res = synth->measurePerformance(std::chrono::milliseconds(250));
  //      CHECK(Tests::getMaxLevel(std::get<0>(res)) == 0);
  //    }
  //  }

  //  WHEN("Init Preset with Mixer A up, is Loaded")
  //  {
  //    XMLPresetLoader::loadTestPresetFromBank(synth.get(), "xml-banks", "InitWithAMix", *settingBasePtr);
  //    THEN("Note Played produces Sound")
  //    {
  //      synth->doMidi({ 0x90, 127, 127 });
  //      auto res = synth->measurePerformance(std::chrono::milliseconds(250));
  //      CHECK(Tests::getMaxLevel(std::get<0>(res)) > 0.2f);
  //    }
  //  }
}
