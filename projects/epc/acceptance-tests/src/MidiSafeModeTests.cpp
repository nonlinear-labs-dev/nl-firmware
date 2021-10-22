#include <catch.hpp>
#include <mock/XMLPresetLoader.h>
#include <memory>
#include <synth/C15Synth.h>
#include <AudioEngineOptions.h>
#include <Toolbox.h>
#include <testing/TestHelper.h>
#include <mock/MockSettingsObject.h>
#include <sync/SyncMasterMockRoot.h>
#include <mock/DspHostDualTester.h>
#include <mock/TCDHelpers.h>

TEST_CASE("Midi Safe Mode disabled")
{
  // prepare Configuration
  auto config = nltools::msg::getConfig();
  config.useEndpoints
      = { nltools::msg::EndPoint::Playground, nltools::msg::EndPoint::AudioEngine, nltools::msg::EndPoint::BeagleBone };
  config.offerEndpoints
      = { nltools::msg::EndPoint::Playground, nltools::msg::EndPoint::AudioEngine, nltools::msg::EndPoint::BeagleBone };
  nltools::msg::init(config);
  auto options = Tests::createEmptyAudioEngineOptions();
  auto synth = std::make_unique<C15Synth>(options.get());
  DspHostDualTester tester{ synth->getDsp() };
  MockSettingsObject settings(&SyncMasterMockRoot::get());

  // prepare Scenario
  constexpr int MeasureTimeMs = 100;
  constexpr int NumberOfNotes = 3;
  constexpr int Notes[NumberOfNotes] = { 48, 52, 55 };

  // prepare Preset
  auto settingBasePtr = static_cast<Settings*>(&settings);
  XMLPresetLoader::loadTestPresetFromBank(synth.get(), "xml-banks", "SplitPlateau", *settingBasePtr);
  synth->measurePerformance(std::chrono::milliseconds(MeasureTimeMs));

  // prepare Midi Settings
  using tMSG = nltools::msg::Setting::MidiSettingsMessage;
  tMSG msg;
  TestHelper::updateMappingForHW(msg.routings, tMSG::RoutingIndex::Notes, tMSG::RoutingAspect::LOCAL, true);
  msg.localEnable = false;
  msg.safeMode = false;
  synth->onMidiSettingsMessage(msg);

  // play TCD Notes
  for(auto note : Notes)
  {
    synth->doTcd(TCD_HELPER::createKeyPosEvent(note));
    synth->doTcd(TCD_HELPER::createKeyDownEvent(127, 127));
  }

  WHEN("multiple Notes played with Split Sound and Local disabled")
  {
    synth->measurePerformance(std::chrono::milliseconds(MeasureTimeMs));

    THEN("produces no Sound")
    {
      CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
    }
  }

  WHEN("multiple Notes played/held with Split Sound and Local re-enabled")
  {
    synth->measurePerformance(std::chrono::milliseconds(MeasureTimeMs));

    // update settings (Notes still active)
    msg.localEnable = true;
    synth->onMidiSettingsMessage(msg);
    synth->measurePerformance(std::chrono::milliseconds(MeasureTimeMs));

    // release TCD Notes
    for(auto note : Notes)
    {
      synth->doTcd(TCD_HELPER::createKeyPosEvent(note));
      synth->doTcd(TCD_HELPER::createKeyUpEvent(127, 127));
    }

    synth->measurePerformance(std::chrono::milliseconds(MeasureTimeMs));

    // play TCD Notes (again)
    for(auto note : Notes)
    {
      synth->doTcd(TCD_HELPER::createKeyPosEvent(note));
      synth->doTcd(TCD_HELPER::createKeyDownEvent(127, 127));
    }

    THEN("multiple Notes played again produces multiple active Voices")
    {
      synth->measurePerformance(std::chrono::milliseconds(MeasureTimeMs));
      CHECK(tester.getActiveVoices(VoiceGroup::Global) == NumberOfNotes);
    }
  }
}
