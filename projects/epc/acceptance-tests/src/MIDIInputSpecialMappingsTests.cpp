#include <catch.hpp>
#include <synth/input/InputEventStage.h>
#include <synth/C15Synth.h>
#include <mock/MockDSPHosts.h>

MidiRuntimeOptions createSpecialSettings()
{
  MidiRuntimeOptions options;
  nltools::msg::Setting::MidiSettingsMessage msg;
  msg.receiveNotes = true;
  msg.receiveControllers = true;
  msg.receiveProgramChange = true;
  msg.receiveChannel = MidiReceiveChannel::Omni;
  msg.receiveSplitChannel = MidiReceiveChannelSplit::Omni;

  msg.sendControllers = true;
  msg.sendProgramChange = true;
  msg.sendNotes = true;
  msg.sendChannel = MidiSendChannel::CH_1;
  msg.sendSplitChannel = MidiSendChannelSplit::CH_1;

  msg.localNotes = true;
  msg.localControllers = true;

  msg.bendercc = BenderCC::Pitchbend;
  msg.aftertouchcc = AftertouchCC::ChannelPressure;
  msg.pedal1cc = PedalCC::CC20;
  msg.pedal2cc = PedalCC::CC21;
  msg.pedal3cc = PedalCC::CC22;
  msg.pedal4cc = PedalCC::CC23;
  msg.ribbon1cc = RibbonCC::CC24;
  msg.ribbon2cc = RibbonCC::CC25;

  options.update(msg);
  return options;
}

namespace SecTests
{
  class SplitDSPMock : public MockDSPHost
  {
   public:
    using MockDSPHost::MockDSPHost;
    SoundType getType() override
    {
      return SoundType::Split;
    }

    VoiceGroup getSplitPartForKey(int key) override
    {
      if(key == 64)
        return VoiceGroup::Global;

      return key > 64 ? VoiceGroup::II : VoiceGroup::I;
    }
  };
}

TEST_CASE("Secondary Channel", "[MIDI][TCD]")
{
  constexpr static uint8_t BASE_TCD = 0b11100000;
  constexpr static uint8_t TCD_KEY_POS = 13;
  constexpr static uint8_t TCD_KEY_DOWN = 14;
  constexpr static uint8_t TCD_KEY_UP = 15;
  constexpr static uint8_t TCD_UNUSED_VAL = 0x00;

  //Prepare MIDI Options
  auto settings = createSpecialSettings();
  settings.setReceiveChannel(MidiReceiveChannel::CH_2);
  settings.setSplitReceiveChannel(MidiReceiveChannelSplit::CH_1);
  settings.setSendChannel(MidiSendChannel::CH_2);
  settings.setSendSplitChannel(MidiSendChannelSplit::CH_1);

  //Construct Objects
  SecTests::SplitDSPMock host;
  std::vector<nltools::msg::Midi::SimpleMessage> midiOut;
  InputEventStage eventStage { &host, &settings, [&](auto m) { midiOut.emplace_back(m); } };

  WHEN("TCD key is pressed on Part I")
  {
    eventStage.onTCDMessage({ BASE_TCD | TCD_KEY_POS, TCD_UNUSED_VAL, 17 });
    eventStage.onTCDMessage({ BASE_TCD | TCD_KEY_DOWN, 127, 127 });

    THEN("MIDI keyDown was send on Channel 2")
    {
      REQUIRE(midiOut.size() == 2);
      CHECK(midiOut[0].rawBytes[0] == 0xB1);
      CHECK(midiOut[0].rawBytes[1] == 88);
      CHECK(midiOut[0].rawBytes[2] == 0);

      CHECK(midiOut[1].rawBytes[0] == 0x91);
      CHECK(midiOut[1].rawBytes[1] == 17);
      CHECK(midiOut[1].rawBytes[2] == 127);
    }
  }

  WHEN("TCD key is pressed on Part II")
  {
    eventStage.onTCDMessage({ BASE_TCD | TCD_KEY_POS, TCD_UNUSED_VAL, 66 });
    eventStage.onTCDMessage({ BASE_TCD | TCD_KEY_DOWN, 127, 127 });

    THEN("MIDI keyDown was send on Channel 1")
    {
      REQUIRE(midiOut.size() == 2);
      CHECK(midiOut[0].rawBytes[0] == 0xB0);
      CHECK(midiOut[0].rawBytes[1] == 88);
      CHECK(midiOut[0].rawBytes[2] == 0);

      CHECK(midiOut[1].rawBytes[0] == 0x90);
      CHECK(midiOut[1].rawBytes[1] == 66);
      CHECK(midiOut[1].rawBytes[2] == 127);
    }
  }

  WHEN("TCD key is pressed on Overlap")
  {
    eventStage.onTCDMessage({ BASE_TCD | TCD_KEY_POS, TCD_UNUSED_VAL, 64 });
    eventStage.onTCDMessage({ BASE_TCD | TCD_KEY_DOWN, 127, 127 });

    THEN("MIDI keyDown was send on Channel 1 & 2")
    {
      REQUIRE(midiOut.size() == 4);
      CHECK(midiOut[0].rawBytes[0] == 0xB1);
      CHECK(midiOut[0].rawBytes[1] == 88);
      CHECK(midiOut[0].rawBytes[2] == 0);

      CHECK(midiOut[1].rawBytes[0] == 0x91);
      CHECK(midiOut[1].rawBytes[1] == 64);
      CHECK(midiOut[1].rawBytes[2] == 127);

      CHECK(midiOut[2].rawBytes[0] == 0xB0);
      CHECK(midiOut[2].rawBytes[1] == 88);
      CHECK(midiOut[2].rawBytes[2] == 0);

      CHECK(midiOut[3].rawBytes[0] == 0x90);
      CHECK(midiOut[3].rawBytes[1] == 64);
      CHECK(midiOut[3].rawBytes[2] == 127);
    }
  }
}