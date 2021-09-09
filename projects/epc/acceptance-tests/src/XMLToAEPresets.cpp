#include <catch.hpp>
#include <mock/XMLPresetLoader.h>
#include <memory>
#include <synth/C15Synth.h>
#include <AudioEngineOptions.h>
#include <Toolbox.h>
#include <testing/TestHelper.h>
#include <mock/UpdateDocumentMasterMock.h>
#include <mock/MockSettingsObject.h>

TEST_CASE("Load XML Preset into AE")
{
  auto config = nltools::msg::getConfig();
  config.useEndpoints = {nltools::msg::EndPoint::Playground, nltools::msg::EndPoint::AudioEngine, nltools::msg::EndPoint::BeagleBone};
  config.offerEndpoints = {nltools::msg::EndPoint::Playground, nltools::msg::EndPoint::AudioEngine, nltools::msg::EndPoint::BeagleBone};
  nltools::msg::init(config);

  auto options = Tests::createEmptyAudioEngineOptions();
  auto synth = std::make_unique<C15Synth>(options.get());

  UpdateDocumentMasterMock master;
  MockSettingsObject settings(&master);

  //Prepare Midi Settings
  {
    using tMSG = nltools::msg::Setting::MidiSettingsMessage;
    tMSG msg;
    msg.receiveChannel = MidiReceiveChannel::CH_1;
    TestHelper::updateMappingForHW(msg.routings,
                                   tMSG::RoutingIndex::Notes,
                                   tMSG::RoutingAspect::RECEIVE_PRIMARY,
                                   true);

    synth->onMidiSettingsMessage(msg);
  }

  auto settingBasePtr = static_cast<Settings*>(&settings);

  WHEN("Init Preset is Loaded")
  {
    XMLPresetLoader::loadTestPresetFromBank(synth.get(), "xml-banks", "Init", *settingBasePtr);
    THEN("Note Played produces no Sound")
    {
      synth->doMidi({ 0x90, 127, 127 });
      auto res = synth->measurePerformance(std::chrono::milliseconds(250));
      CHECK(Tests::getMaxLevel(std::get<0>(res)) == 0);
    }
  }

  WHEN("Init Preset with Mixer A up, is Loaded")
  {
    XMLPresetLoader::loadTestPresetFromBank(synth.get(), "xml-banks", "InitWithAMix", *settingBasePtr);
    THEN("Note Played produces Sound")
    {
      synth->doMidi({ 0x90, 127, 127 });
      auto res = synth->measurePerformance(std::chrono::milliseconds(250));
      CHECK(Tests::getMaxLevel(std::get<0>(res)) > 0.2f);
    }
  }
}
