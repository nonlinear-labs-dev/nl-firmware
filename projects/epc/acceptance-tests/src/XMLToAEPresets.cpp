#include <catch.hpp>
#include <mock/XMLPresetLoader.h>
#include <memory>
#include <synth/C15Synth.h>
#include <AudioEngineOptions.h>
#include <Toolbox.h>
#include <testing/TestHelper.h>
#include <mock/MockSettingsObject.h>
#include <sync/SyncMasterMockRoot.h>

TEST_CASE("Load XML Preset into AE")
{
  auto config = nltools::msg::getConfig();
  config.useEndpoints = { nltools::msg::EndPoint::Playground, nltools::msg::EndPoint::AudioEngine,
                          nltools::msg::EndPoint::BeagleBone, nltools::msg::EndPoint::Playcontroller };
  config.offerEndpoints = { nltools::msg::EndPoint::Playground, nltools::msg::EndPoint::AudioEngine,
                            nltools::msg::EndPoint::BeagleBone, nltools::msg::EndPoint::Playcontroller };
  nltools::msg::init(config);

  auto options = Tests::createEmptyAudioEngineOptions();
  auto synth = std::make_unique<C15Synth>(options.get());

  MockSettingsObject settings("", &SyncMasterMockRoot::get());

  //Prepare Midi Settings
  {
    using tMSG = nltools::msg::Setting::MidiSettingsMessage;
    tMSG msg;
    msg.receiveChannel = MidiReceiveChannel::CH_1;
    TestHelper::updateMappingForHW(msg.routings, tMSG::RoutingIndex::Notes, tMSG::RoutingAspect::RECEIVE_PRIMARY, true);

    synth->onMidiSettingsMessage(msg);
  }

  auto settingBasePtr = dynamic_cast<Settings*>(&settings);
  REQUIRE(settingBasePtr);

  WHEN("Init Preset is Loaded")
  {
    CHECK_NOTHROW(XMLPresetLoader::loadTestPresetFromBank(synth.get(), "xml-banks", "Init", *settingBasePtr));
    THEN("Note Played produces no Sound")
    {
      synth->doMidi({ { 0x90, 127, 127 } });
      auto res = synth->measurePerformance(std::chrono::milliseconds(250));
      CHECK(Tests::getMaxLevel(std::get<0>(res)) == 0);
    }
  }

  WHEN("Init Preset with Mixer A up, is Loaded")
  {
    CHECK_NOTHROW(XMLPresetLoader::loadTestPresetFromBank(synth.get(), "xml-banks", "InitWithAMix", *settingBasePtr));
    THEN("Note Played produces Sound")
    {
      synth->doMidi({ { 0x90, 127, 127 } });
      auto res = synth->measurePerformance(std::chrono::milliseconds(250));
      CHECK(Tests::getMaxLevel(std::get<0>(res)) > 0.2f);
    }
  }
}