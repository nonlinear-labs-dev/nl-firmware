#include <catch.hpp>
#include <synth/input/InputEventStage.h>
#include <synth/C15Synth.h>
#include <mock/MockDSPHosts.h>

MidiRuntimeOptions createTCDSettings()
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

constexpr static uint8_t BASE_TCD = 0b11100000;
constexpr static uint8_t TCD_KEY_POS = 13;
constexpr static uint8_t TCD_KEY_DOWN = 14;
constexpr static uint8_t TCD_KEY_UP = 15;
constexpr static uint8_t TCD_UNUSED_VAL = 0x00;

TEST_CASE("TCD in leads to key down and send midi", "[MIDI][TCD]")
{
  std::vector<nltools::msg::Midi::SimpleMessage> sendMessages;
  PassOnKeyDownHost dsp { 17, 1.0, VoiceGroup::I };
  auto settings = createTCDSettings();
  InputEventStage eventStage { &dsp, &settings,
                               [&](nltools::msg::Midi::SimpleMessage msg) { sendMessages.push_back(msg); } };

  WHEN("Keypos and KeyDown is received")
  {
    eventStage.onTCDMessage({ BASE_TCD | TCD_KEY_POS, TCD_UNUSED_VAL, 17 });
    eventStage.onTCDMessage({ BASE_TCD | TCD_KEY_DOWN, 127, 127 });

    THEN("DSP got notified")
    {
      CHECK(dsp.didReceiveKeyDown());
    }

    THEN("MIDI got send")
    {
      REQUIRE(sendMessages.size() == 2);
      CHECK(sendMessages[0].rawBytes[0] == 0xB0);
      CHECK(sendMessages[0].rawBytes[1] == 88);
      CHECK(
          sendMessages[0].rawBytes[2]
          == 0);  // i would have expected 127 here, but apparently the valid 14 bit out range is only up to 16256 which has the last 7 bits set to 0

      CHECK(sendMessages[1].rawBytes[0] == 0x90);
      CHECK(sendMessages[1].rawBytes[1] == 17);
      CHECK(sendMessages[1].rawBytes[2] == 127);
    }
  }
}