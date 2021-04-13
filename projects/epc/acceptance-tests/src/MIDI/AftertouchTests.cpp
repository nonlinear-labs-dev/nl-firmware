#include <catch.hpp>
#include <mock/InputEventStageTester.h>
#include <mock/MockDSPHosts.h>

TEST_CASE("Aftertouch Mappings", "[MIDI][TCD]")
{
  bool receivedHW = false;
  ConfigureableDSPHost host {};
  host.setType(SoundType::Single);
  host.setOnHWChangedCB(
      [&](int hwID, float hwPos)
      {
        CHECK(hwID == 5);
        CHECK(hwPos == 1.0f);
        receivedHW = true;
      });

  std::vector<nltools::msg::Midi::SimpleMessage> sendMidiMessages;
  MidiRuntimeOptions settings;
  InputEventStage eventStage(&host, &settings, [&](auto msg) { sendMidiMessages.push_back(msg); });

  //set settings to not interfere with CC01
  {
    nltools::msg::Setting::MidiSettingsMessage msg;
    msg.receiveControllers = true;
    msg.sendControllers = true;
    msg.receiveChannel = MidiReceiveChannel::CH_1;
    msg.sendChannel = MidiSendChannel::CH_1;
    msg.pedal1cc = PedalCC::CC02;
    msg.pedal2cc = PedalCC::CC02;
    msg.pedal3cc = PedalCC::CC02;
    msg.pedal4cc = PedalCC::CC02;
    msg.ribbon1cc = RibbonCC::CC02;
    msg.ribbon2cc = RibbonCC::CC02;
    msg.bendercc = BenderCC::CC02;
    settings.update(msg);
  }

  WHEN("Mapped to Channel Pressure")
  {
    settings.setAftertouchCC(AftertouchCC::ChannelPressure);

    WHEN("Send MIDI Channel Pressure")
    {
      eventStage.onMIDIMessage({ 0xD0, 127, 127 });
      CHECK(receivedHW);
    }

    WHEN("Send CC01")
    {
      eventStage.onMIDIMessage({ 0xB0, 0x01, 127 });
      CHECK_FALSE(receivedHW);
    }
  }

  WHEN("Mapped to CC01")
  {
    settings.setAftertouchCC(AftertouchCC::CC01);

    WHEN("Send MIDI Channel Pressure")
    {
      eventStage.onMIDIMessage({ 0xD0, 127, 127 });
      CHECK_FALSE(receivedHW);
    }

    WHEN("Send CC01")
    {
      eventStage.onMIDIMessage({ 0xB0, 0x01, 127 });
      CHECK(receivedHW);
    }
  }

  WHEN("Mapped to Special Case PitchbendDown")
  {
    settings.setAftertouchCC(AftertouchCC::PitchbendDown);
    host.setOnHWChangedCB(
        [&](int hwID, float hwPos)
        {
          CHECK(hwID == 5);
          CHECK(hwPos == 1.0f);
          receivedHW = true;
        });

    WHEN("Send MIDI Pitchbend, -1.0")
    {
      eventStage.onMIDIMessage({ 0xE0, 0, 0 });
      CHECK(receivedHW);
    }

    WHEN("Send CC01")
    {
      eventStage.onMIDIMessage({ 0xB0, 0x01, 127 });
      CHECK_FALSE(receivedHW);
    }
  }

  WHEN("Mapped to Special Case PitchbendUp")
  {
    settings.setAftertouchCC(AftertouchCC::PitchbendUp);
    host.setOnHWChangedCB(
        [&](int hwID, float hwPos)
        {
          CHECK(hwID == 5);
          CHECK(hwPos == 1.0f);
          receivedHW = true;
        });

    WHEN("Send MIDI Channel Pitchbend")
    {
      eventStage.onMIDIMessage({ 0xE0, 127, 127 });
      CHECK(receivedHW);
    }

    WHEN("Send CC01")
    {
      eventStage.onMIDIMessage({ 0xB0, 0x01, 127 });
      CHECK_FALSE(receivedHW);
    }
  }
}