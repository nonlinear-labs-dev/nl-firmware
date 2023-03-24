#include <testing/TestHelper.h>
#include <memory>
#include <synth/C15Synth.h>
#include <AudioEngineOptions.h>
#include <Toolbox.h>
#include <mock/DspHostDualTester.h>
#include <sync/SyncMasterMockRoot.h>
#include <mock/XMLPresetLoader.h>
#include <mock/TCDHelpers.h>
#include "CompileTimeOptions.h"
#include <presets/Bank.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Reset on convert")
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

  //Prepare Midi Settings
  DspHostDualTester tester { synth->getDsp() };

  // prepare Scenario
  constexpr int NumberOfNotes = 3;
  constexpr int Notes[NumberOfNotes] = { 48, 52, 55 };

  // prepare Midi Settings
  using tMSG = nltools::msg::Setting::MidiSettingsMessage;
  tMSG msg {};
  TestHelper::updateMappingForHW(msg.routings, tMSG::RoutingIndex::Notes, tMSG::RoutingAspect::LOCAL, true);
  msg.localEnable = true;
  synth->onMidiSettingsMessage(msg);
  auto eb = app->getPresetManager()->getEditBuffer();

  EditBufferUseCases ebUseCases(*eb);

  WHEN("Split is Loaded")
  {
    XMLPresetLoader::loadFirstPresetOfBank(app.get(), "SplitPlateau.xml", synth.get());
    synth->measurePerformance(std::chrono::milliseconds(10));

    THEN("Notes are held")
    {
      for(auto n : Notes)
      {
        synth->doTcd(TCD_HELPER::createKeyPosEvent(n));
        synth->doTcd(TCD_HELPER::createKeyDownEvent(127, 127));
      }

      synth->measurePerformance(std::chrono::milliseconds(10));
      CHECK(tester.getActiveVoices(VoiceGroup::Global) == NumberOfNotes);

      ebUseCases.convertToSingle(VoiceGroup::I);
      XMLPresetLoader::sendToAE(eb, synth.get());

      WHEN("Sound was converted")
      {
        synth->measurePerformance(std::chrono::milliseconds(10));
        REQUIRE(synth->getDsp()->getType() == SoundType::Single);
        CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
      }
    }
  }

  WHEN("Single is Loaded")
  {
    XMLPresetLoader::loadFirstPresetOfBank(app.get(), "InitWithAMix.xml", synth.get());
    synth->measurePerformance(std::chrono::milliseconds(10));

    THEN("Notes are held")
    {
      for(auto n : Notes)
      {
        synth->doTcd(TCD_HELPER::createKeyPosEvent(n));
        synth->doTcd(TCD_HELPER::createKeyDownEvent(127, 127));
      }

      synth->measurePerformance(std::chrono::milliseconds(10));
      CHECK(tester.getActiveVoices(VoiceGroup::Global) == NumberOfNotes);
      ebUseCases.convertToSplit(VoiceGroup::I);
      XMLPresetLoader::sendToAE(eb, synth.get());

      WHEN("Sound was converted")
      {
        synth->measurePerformance(std::chrono::milliseconds(10));
        REQUIRE(synth->getDsp()->getType() == SoundType::Split);
        CHECK(tester.getActiveVoices(VoiceGroup::Global) == 0);
      }
    }
  }
}