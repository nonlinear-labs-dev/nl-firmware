#include <catch.hpp>
#include <synth/input/InputEventStage.h>
#include <synth/C15Synth.h>
#include <mock/MockDSPHosts.h>
#include <mock/InputEventStageTester.h>

TEST_CASE("Interface correct", "[MIDI]")
{
  MockDSPHost host;
  MidiRuntimeOptions options;
  InputEventStage eventStage(&host, &options, [](auto) {});
  InputEventStageTester testObject(&eventStage);

  WHEN("Mappings: prim = none, sec = Common")
  {

    {
      nltools::msg::Setting::MidiSettingsMessage msg;
      msg.receiveNotes = true;
      msg.receiveChannel = MidiReceiveChannel::None;
      msg.receiveSplitChannel = MidiReceiveChannelSplit::Common;
      options.update(msg);
    }

    MIDIDecoder testDecoder(&host, &options);
    testDecoder.decode({ 0x90, 127, 127 });
    auto ret = testObject.getInterfaceFromDecoder(&testDecoder);
    CHECK(ret == DSPInterface::InputSource::Unknown);
  }

  WHEN("Mappings: prim = 1, sec = 1")
  {
    {
      nltools::msg::Setting::MidiSettingsMessage msg;
      msg.receiveNotes = true;
      msg.receiveChannel = MidiReceiveChannel::CH_1;
      msg.receiveSplitChannel = MidiReceiveChannelSplit::CH_1;
      options.update(msg);
    }

    MIDIDecoder testDecoder(&host, &options);
    testDecoder.decode({ 0x90, 127, 127 });
    auto ret = testObject.getInterfaceFromDecoder(&testDecoder);
    CHECK(ret == DSPInterface::InputSource::Both);
  }

  WHEN("Mappings: prim = 1, sec = 2")
  {
    {
      nltools::msg::Setting::MidiSettingsMessage msg;
      msg.receiveNotes = true;
      msg.receiveChannel = MidiReceiveChannel::CH_1;
      msg.receiveSplitChannel = MidiReceiveChannelSplit::CH_2;
      options.update(msg);
    }

    MIDIDecoder testDecoder(&host, &options);
    WHEN("Send on Channel 1")
    {
      testDecoder.decode({ 0x90, 127, 127 });
      auto ret = testObject.getInterfaceFromDecoder(&testDecoder);
      CHECK(ret == DSPInterface::InputSource::Primary);
    }

    WHEN("Send on Channel 2")
    {
      testDecoder.decode({ 0x91, 127, 127 });
      auto ret = testObject.getInterfaceFromDecoder(&testDecoder);
      CHECK(ret == DSPInterface::InputSource::Secondary);
    }
  }
}