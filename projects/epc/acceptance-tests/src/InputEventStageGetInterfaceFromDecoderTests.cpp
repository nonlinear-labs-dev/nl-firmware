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
    auto ret = testObject.getInterfaceFromDecoder(testDecoder.getChannel());
    CHECK(ret.m_source == DSPInterface::InputSource::Unknown);
    CHECK(ret.m_state == DSPInterface::InputState::Invalid);
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
    auto ret = testObject.getInterfaceFromDecoder(testDecoder.getChannel());
    CHECK(ret.m_source == DSPInterface::InputSource::Both);
    CHECK(ret.m_state == DSPInterface::InputState::Separate);
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
      auto ret = testObject.getInterfaceFromDecoder(testDecoder.getChannel());
      CHECK(ret.m_source == DSPInterface::InputSource::Primary);
      CHECK(ret.m_state == DSPInterface::InputState::Separate);
    }

    WHEN("Send on Channel 2")
    {
      testDecoder.decode({ 0x91, 127, 127 });
      auto ret = testObject.getInterfaceFromDecoder(testDecoder.getChannel());
      CHECK(ret.m_source == DSPInterface::InputSource::Secondary);
      CHECK(ret.m_state == DSPInterface::InputState::Separate);
    }
  }
}

namespace CCBitDetail
{
  void setSettings(MidiRuntimeOptions& options, MidiReceiveChannel r, MidiReceiveChannelSplit rs)
  {
    nltools::msg::Setting::MidiSettingsMessage msg;
    msg.receiveNotes = true;
    msg.receiveChannel = r;
    msg.receiveSplitChannel = rs;
    options.update(msg);
  }
}

TEST_CASE("MIDI Channel Mapping Tests", "[MIDI]")
{
  MockDSPHost host;
  MidiRuntimeOptions options;
  InputEventStage eventStage { &host, &options, [](auto) {} };
  InputEventStageTester t(&eventStage);

  // specific tests
  WHEN("Settings receive both none")
  {
    CCBitDetail::setSettings(options, MidiReceiveChannel::None, MidiReceiveChannelSplit::None);
    for(auto channel = 0; channel < 16; channel++)
    {
      auto testChannel = static_cast<MidiReceiveChannel>(channel + 2);
      auto ret = t.getInterfaceFromDecoder(testChannel);
      CHECK(ret.m_source == DSPInterface::InputSource::Unknown);
      CHECK(ret.m_state == DSPInterface::InputState::Invalid);
    }
  }

  WHEN("Settings receive none and common")
  {
    CCBitDetail::setSettings(options, MidiReceiveChannel::None, MidiReceiveChannelSplit::Common);
    for(auto channel = 0; channel < 16; channel++)
    {
      auto testChannel = static_cast<MidiReceiveChannel>(channel + 2);
      auto ret = t.getInterfaceFromDecoder(testChannel);
      CHECK(ret.m_source == DSPInterface::InputSource::Unknown);
      CHECK(ret.m_state == DSPInterface::InputState::Invalid);
    }
  }

  WHEN("Settings receive omni and common")
  {
    CCBitDetail::setSettings(options, MidiReceiveChannel::Omni, MidiReceiveChannelSplit::Common);
    for(auto channel = 0; channel < 16; channel++)
    {
      auto testChannel = static_cast<MidiReceiveChannel>(channel + 2);
      auto ret = t.getInterfaceFromDecoder(testChannel);
      CHECK(ret.m_source == DSPInterface::InputSource::Primary);
      CHECK(ret.m_state == DSPInterface::InputState::Singular);
    }
  }

  WHEN("Settings receive 1 and common")
  {
    CCBitDetail::setSettings(options, MidiReceiveChannel::CH_1, MidiReceiveChannelSplit::Common);
    auto ret = t.getInterfaceFromDecoder(MidiReceiveChannel::CH_1);
    CHECK(ret.m_source == DSPInterface::InputSource::Primary);
    CHECK(ret.m_state == DSPInterface::InputState::Singular);

    for(auto channel = 1; channel < 16; channel++)
    {
      auto testChannel = static_cast<MidiReceiveChannel>(channel + 2);
      auto ret2 = t.getInterfaceFromDecoder(testChannel);
      CHECK(ret2.m_source == DSPInterface::InputSource::Unknown);
      CHECK(ret2.m_state == DSPInterface::InputState::Invalid);
    }
  }

  WHEN("Settings receive Omni and 2")
  {
    CCBitDetail::setSettings(options, MidiReceiveChannel::Omni, MidiReceiveChannelSplit::CH_2);

    auto ret = t.getInterfaceFromDecoder(MidiReceiveChannel::CH_1);
    CHECK(ret.m_source == DSPInterface::InputSource::Primary);
    CHECK(ret.m_state == DSPInterface::InputState::Separate);

    auto ret2 = t.getInterfaceFromDecoder(MidiReceiveChannel::CH_2);
    CHECK(ret2.m_source == DSPInterface::InputSource::Both);
    CHECK(ret2.m_state == DSPInterface::InputState::Separate);

    for(auto channel = 2; channel < 16; channel++)
    {
      auto testChannel = static_cast<MidiReceiveChannel>(channel + 2);
      auto ret3 = t.getInterfaceFromDecoder(testChannel);
      CHECK(ret3.m_source == DSPInterface::InputSource::Primary);
      CHECK(ret3.m_state == DSPInterface::InputState::Separate);
    }
  }

  WHEN("Settings receive 2 and 3")
  {
    CCBitDetail::setSettings(options, MidiReceiveChannel::CH_2, MidiReceiveChannelSplit::CH_3);

    auto ret = t.getInterfaceFromDecoder(MidiReceiveChannel::CH_1);
    CHECK(ret.m_source == DSPInterface::InputSource::Unknown);
    CHECK(ret.m_state == DSPInterface::InputState::Invalid);

    auto ret2 = t.getInterfaceFromDecoder(MidiReceiveChannel::CH_2);
    CHECK(ret2.m_source == DSPInterface::InputSource::Primary);
    CHECK(ret2.m_state == DSPInterface::InputState::Separate);

    auto ret3 = t.getInterfaceFromDecoder(MidiReceiveChannel::CH_3);
    CHECK(ret3.m_source == DSPInterface::InputSource::Secondary);
    CHECK(ret3.m_state == DSPInterface::InputState::Separate);

    for(auto channel = 3; channel < 16; channel++)
    {
      WHEN("Received Channel is " + std::to_string(channel))
      {
        auto testChannel = static_cast<MidiReceiveChannel>(channel + 2);
        auto ret4 = t.getInterfaceFromDecoder(testChannel);
        CHECK(ret4.m_source == DSPInterface::InputSource::Unknown);
        CHECK(ret4.m_state == DSPInterface::InputState::Invalid);
      }
    }
  }

  WHEN("Settings receive 3 and Omni")
  {
    CCBitDetail::setSettings(options, MidiReceiveChannel::CH_3, MidiReceiveChannelSplit::Omni);

    for(auto channel = 0; channel < 16; channel++)
    {
      WHEN("Received Channel is " + std::to_string(channel))
      {
        auto testChannel = static_cast<MidiReceiveChannel>(channel + 2);
        auto ret = t.getInterfaceFromDecoder(testChannel);

        if(testChannel == MidiReceiveChannel::CH_3)
        {
          CHECK(ret.m_source == DSPInterface::InputSource::Both);
          CHECK(ret.m_state == DSPInterface::InputState::Separate);
        }
        else
        {
          CHECK(ret.m_source == DSPInterface::InputSource::Secondary);
          CHECK(ret.m_state == DSPInterface::InputState::Separate);
        }
      }
    }
  }

  WHEN("Settings receive Omni and Omni")
  {
    CCBitDetail::setSettings(options, MidiReceiveChannel::Omni, MidiReceiveChannelSplit::Omni);

    for(auto channel = 0; channel < 16; channel++)
    {
      WHEN("Received Channel is " + std::to_string(channel))
      {
        auto testChannel = static_cast<MidiReceiveChannel>(channel + 2);
        auto ret = t.getInterfaceFromDecoder(testChannel);
        CHECK(ret.m_source == DSPInterface::InputSource::Both);
        CHECK(ret.m_state == DSPInterface::InputState::Separate);
      }
    }
  }
}