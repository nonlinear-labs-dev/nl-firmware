#include <testing/TestHelper.h>
#include <Types.h>
#include <mock/MockDSPHosts.h>
#include <mock/TCDHelpers.h>
#include <Toolbox.h>
#include <AudioEngineOptions.h>
#include <mock/DspHostDualTester.h>

TEST_CASE("Send HW-Change only in Split Sound on Split Channel")
{
  constexpr static uint8_t BASE_TCD = 0b11100000;
  constexpr static uint8_t Aftertouch = 0b00000101;
  constexpr static auto sixteenThousand = 0b11111010000000;

  constexpr MidiEvent fullPressureTCDEvent
      = { { BASE_TCD | Aftertouch, (uint8_t) (sixteenThousand >> 7), (uint8_t) (sixteenThousand & 127) } };

  ConfigureableDSPHost host {};
  host.setType(SoundType::Single);

  std::vector<nltools::msg::Midi::SimpleMessage> sendMidiMessages;
  MidiRuntimeOptions settings;
  InputEventStage eventStage(
      &host, &settings, [] {}, [&](auto msg) { sendMidiMessages.push_back(msg); }, [](auto) {});

  //set settings to not interfere with CC01
  {
    nltools::msg::Setting::MidiSettingsMessage msg;
    msg.receiveChannel = MidiReceiveChannel::CH_1;
    msg.sendChannel = MidiSendChannel::CH_1;
    msg.sendSplitChannel = MidiSendChannelSplit::CH_2;

    msg.routings = TestHelper::createFullMappings(true);

    msg.highResCCEnabled = false;
    msg.pedal1cc = PedalCC::CC02;
    msg.pedal2cc = PedalCC::CC02;
    msg.pedal3cc = PedalCC::CC02;
    msg.pedal4cc = PedalCC::CC02;
    msg.ribbon1cc = RibbonCC::CC02;
    msg.ribbon2cc = RibbonCC::CC02;
    msg.bendercc = BenderCC::CC02;
    msg.aftertouchcc = AftertouchCC::CC30;
    settings.update(msg);
  }

  auto sendTCDHWChange = [&]() { eventStage.onTCDMessage(fullPressureTCDEvent); };

  auto doTests = [&](const std::map<SoundType, int>& expected)
  {
    WHEN("Sound is Single")
    {
      host.setType(SoundType::Single);

      WHEN("TCD HW Change is received")
      {
        sendTCDHWChange();

        THEN("HW Change got send on Primary Channel")
        {
          CHECK(sendMidiMessages.size() == expected.at(SoundType::Single));
        }
      }
    }

    WHEN("Sound is Layer")
    {
      host.setType(SoundType::Layer);

      WHEN("TCD HW Change is received")
      {
        sendTCDHWChange();

        THEN("HW Change got send on Primary Channel")
        {
          CHECK(sendMidiMessages.size() == expected.at(SoundType::Layer));
        }
      }
    }

    WHEN("Sound is Split")
    {
      host.setType(SoundType::Split);

      WHEN("TCD HW Change is received")
      {
        sendTCDHWChange();

        THEN("HW Change got send on Primary and Split Channels")
        {
          CHECK(sendMidiMessages.size() == expected.at(SoundType::Split));
        }
      }
    }
  };

  WHEN("Split Channel is Channel 2")
  {
    std::map<SoundType, int> expected = { { SoundType::Single, 1 }, { SoundType::Layer, 1 }, { SoundType::Split, 2 } };

    settings.setSendSplitChannel(MidiSendChannelSplit::CH_2);
    doTests(expected);
  }

  WHEN("Split Channel is NONE")
  {
    std::map<SoundType, int> expected = { { SoundType::Single, 1 }, { SoundType::Layer, 1 }, { SoundType::Split, 1 } };

    settings.setSendSplitChannel(MidiSendChannelSplit::None);
    doTests(expected);
  }

  WHEN("Split Channel is Common")
  {
    std::map<SoundType, int> expected = { { SoundType::Single, 1 }, { SoundType::Layer, 1 }, { SoundType::Split, 1 } };

    settings.setSendSplitChannel(MidiSendChannelSplit::Common);
    doTests(expected);
  }
}

TEST_CASE("Send Note Off with real Synth when local is off")
{
  using namespace std::chrono_literals;

  auto options = Tests::createEmptyAudioEngineOptions();
  auto synth = std::make_unique<C15Synth>(options.get());
  DspHostDualTester host { synth->getDsp() };
  host.applyMalformedSplitPreset({}, {});
  host.setSplit(VoiceGroup::I, 0.5);
  host.setSplit(VoiceGroup::II, 0.5 + (1.0 / 61.0));
  MidiRuntimeOptions settings;

  std::vector<nltools::msg::Midi::SimpleMessage> sendMidiMessages;
  InputEventStage eventStage(
      synth->getDsp(), &settings, [] {}, [&](auto msg) { sendMidiMessages.push_back(msg); }, [](auto) {});

  {
    nltools::msg::Setting::MidiSettingsMessage msg;
    msg.sendChannel = MidiSendChannel::CH_1;
    msg.sendSplitChannel = MidiSendChannelSplit::CH_2;
    msg.receiveChannel = MidiReceiveChannel::CH_3;
    msg.receiveSplitChannel = MidiReceiveChannelSplit::CH_4;

    msg.routings = TestHelper::createFullMappings(true);

    msg.highResCCEnabled = false;
    msg.highVeloCCEnabled = false;

    msg.pedal1cc = PedalCC::CC02;
    msg.pedal2cc = PedalCC::CC02;
    msg.pedal3cc = PedalCC::CC02;
    msg.pedal4cc = PedalCC::CC02;
    msg.ribbon1cc = RibbonCC::CC02;
    msg.ribbon2cc = RibbonCC::CC02;
    msg.bendercc = BenderCC::CC02;
    msg.aftertouchcc = AftertouchCC::CC30;

    //disable global local and notes local
    msg.localEnable = false;
    using tR = nltools::msg::Setting::MidiSettingsMessage::RoutingIndex;
    using tA = nltools::msg::Setting::MidiSettingsMessage::RoutingAspect;
    TestHelper::updateMappingForHW(msg.routings, tR::Notes, tA::LOCAL, false);

    settings.update(msg);
  }

  WHEN("Key Down Sends one Midi Event on Channel 1")
  {
    eventStage.onTCDMessage(TCD_HELPER::createKeyPosEvent(17));
    eventStage.onTCDMessage(TCD_HELPER::createKeyDownEvent(127, 127));

    THEN("midi got send")
    {
      REQUIRE(sendMidiMessages.size() == 1);
      CHECK(MIDI_HELPER::getChannel(sendMidiMessages[0].rawBytes[0]) == 0);
    }

    WHEN("Key Up sends Event")
    {
      eventStage.onTCDMessage(TCD_HELPER::createKeyPosEvent(17));
      eventStage.onTCDMessage(TCD_HELPER::createKeyUpEvent(0, 0));
      REQUIRE(sendMidiMessages.size() == 2);
      CHECK(MIDI_HELPER::getChannel(sendMidiMessages[1].rawBytes[0]) == 0);
    }
  }

  WHEN("Key Down Sends one Midi Event on Channel 2")
  {
    eventStage.onTCDMessage(TCD_HELPER::createKeyPosEvent(107));
    eventStage.onTCDMessage(TCD_HELPER::createKeyDownEvent(127, 127));

    THEN("midi got send")
    {
      REQUIRE(sendMidiMessages.size() == 1);
      CHECK(MIDI_HELPER::getChannel(sendMidiMessages[0].rawBytes[0]) == 1);
    }

    WHEN("Key Up sends Event")
    {
      eventStage.onTCDMessage(TCD_HELPER::createKeyPosEvent(107));
      eventStage.onTCDMessage(TCD_HELPER::createKeyUpEvent(0, 0));

      REQUIRE(sendMidiMessages.size() == 2);
      CHECK(MIDI_HELPER::getChannel(sendMidiMessages[1].rawBytes[0]) == 1);
    }
  }
}

TEST_CASE("Send Note Off when local is off")
{
  auto config = nltools::msg::getConfig();
  config.useEndpoints
      = { nltools::msg::EndPoint::Playground, nltools::msg::EndPoint::AudioEngine, nltools::msg::EndPoint::BeagleBone };
  config.offerEndpoints
      = { nltools::msg::EndPoint::Playground, nltools::msg::EndPoint::AudioEngine, nltools::msg::EndPoint::BeagleBone };
  nltools::msg::init(config);
  auto options = Tests::createEmptyAudioEngineOptions();
  auto synth = std::make_unique<C15Synth>(options.get());
  DspHostDualTester tester { synth->getDsp() };
  tester.applyMalformedSplitPreset({}, {}, 0.5);
  MidiRuntimeOptions settings;

  std::vector<nltools::msg::Midi::SimpleMessage> sendMidiMessages;
  InputEventStage eventStage(
      synth->getDsp(), &settings, [] {}, [&](auto msg) { sendMidiMessages.push_back(msg); }, [](auto) {});

  {
    nltools::msg::Setting::MidiSettingsMessage msg;
    msg.sendChannel = MidiSendChannel::CH_1;
    msg.sendSplitChannel = MidiSendChannelSplit::CH_2;
    msg.receiveChannel = MidiReceiveChannel::CH_3;
    msg.receiveSplitChannel = MidiReceiveChannelSplit::CH_4;

    msg.routings = TestHelper::createFullMappings(true);

    msg.highResCCEnabled = false;
    msg.highVeloCCEnabled = false;

    msg.pedal1cc = PedalCC::CC02;
    msg.pedal2cc = PedalCC::CC02;
    msg.pedal3cc = PedalCC::CC02;
    msg.pedal4cc = PedalCC::CC02;
    msg.ribbon1cc = RibbonCC::CC02;
    msg.ribbon2cc = RibbonCC::CC02;
    msg.bendercc = BenderCC::CC02;
    msg.aftertouchcc = AftertouchCC::CC30;

    //disable global local and notes local
    msg.localEnable = false;
    using tR = nltools::msg::Setting::MidiSettingsMessage::RoutingIndex;
    using tA = nltools::msg::Setting::MidiSettingsMessage::RoutingAspect;
    TestHelper::updateMappingForHW(msg.routings, tR::Notes, tA::LOCAL, false);

    synth->onMidiSettingsMessage(msg);
    settings.update(msg);
  }

  WHEN("Key Down Sends one Midi Event on Channel 1")
  {
    eventStage.onTCDMessage(TCD_HELPER::createKeyPosEvent(17));
    eventStage.onTCDMessage(TCD_HELPER::createKeyDownEvent(127, 127));

    REQUIRE(sendMidiMessages.size() == 1);
    CHECK(MIDI_HELPER::getChannel(sendMidiMessages[0].rawBytes[0]) == 0);

    WHEN("Key Up sends Event")
    {
      eventStage.onTCDMessage(TCD_HELPER::createKeyPosEvent(17));
      eventStage.onTCDMessage(TCD_HELPER::createKeyUpEvent(0, 0));
      REQUIRE(sendMidiMessages.size() == 2);
      CHECK(MIDI_HELPER::getChannel(sendMidiMessages[1].rawBytes[0]) == 0);
    }
  }

  WHEN("Key Down Sends one Midi Event on Channel 2")
  {
    eventStage.onTCDMessage(TCD_HELPER::createKeyPosEvent(107));
    eventStage.onTCDMessage(TCD_HELPER::createKeyDownEvent(127, 127));

    REQUIRE(sendMidiMessages.size() == 1);
    CHECK(MIDI_HELPER::getChannel(sendMidiMessages[0].rawBytes[0]) == 1);

    WHEN("Key Up sends Event")
    {
      eventStage.onTCDMessage(TCD_HELPER::createKeyPosEvent(107));
      eventStage.onTCDMessage(TCD_HELPER::createKeyUpEvent(0, 0));

      REQUIRE(sendMidiMessages.size() == 2);
      CHECK(MIDI_HELPER::getChannel(sendMidiMessages[1].rawBytes[0]) == 1);
    }
  }
}