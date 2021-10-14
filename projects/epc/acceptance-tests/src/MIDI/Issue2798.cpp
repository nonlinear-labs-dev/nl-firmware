#include <testing/TestHelper.h>
#include <synth/input/InputEventStage.h>
#include <mock/MockDSPHosts.h>
#include <nltools/messaging/Message.h>

TEST_CASE("When Notes Local is turned off All Notes off gets send", "[MIDI][TCD]")
{

  std::vector<nltools::msg::Midi::SimpleMessage> sendMidi;
  std::vector<MidiChannelModeMessages> messages;
  MockDSPHost host;
  MidiRuntimeOptions options;
  InputEventStage eventStage(&host, &options, {}, [&](auto m){ sendMidi.emplace_back(m); }, [&](auto c){ messages.emplace_back(c); });

  auto msg = nltools::msg::Setting::MidiSettingsMessage{};
  msg.routings = TestHelper::createFullMappings(true);
  msg.sendChannel = MidiSendChannel::CH_1;
  msg.sendSplitChannel = MidiSendChannelSplit::CH_2;
  options.update(msg);
  auto old = options.getLastReceivedMessage();

  WHEN("Previous Notes Enable is disabled")
  {
    msg.routings = TestHelper::createFullMappings(true);
    TestHelper::updateMappings(msg.routings, nltools::msg::Setting::MidiSettingsMessage::RoutingAspect::SEND_PRIMARY, false);
    TestHelper::updateMappings(msg.routings, nltools::msg::Setting::MidiSettingsMessage::RoutingAspect::SEND_SPLIT, false);
    options.update(msg);

    WHEN("DSPHost is Single")
    {
      host.setType(SoundType::Single);
      eventStage.onMidiSettingsMessageWasReceived(msg, old);

      THEN("All Notes Off was send")
      {
        REQUIRE(sendMidi.size() == 1);
        auto channel = sendMidi[0].rawBytes[0] & 0b00001111;
        CHECK(channel == 0);
        CHECK(sendMidi[0].rawBytes[1] == 123);
        CHECK(sendMidi[0].rawBytes[2] == 0);
      }
    }

    WHEN("DSPHost is Split")
    {
      host.setType(SoundType::Split);
      eventStage.onMidiSettingsMessageWasReceived(msg, old);

      THEN("All Notes Off was send")
      {
        REQUIRE(sendMidi.size() == 2);
        auto channel = sendMidi[0].rawBytes[0] & 0b00001111;
        CHECK(channel == 0);
        CHECK(sendMidi[0].rawBytes[1] == 123);
        CHECK(sendMidi[0].rawBytes[2] == 0);
        auto channelSec = sendMidi[1].rawBytes[0] & 0b00001111;
        CHECK(channelSec == 1);
        CHECK(sendMidi[1].rawBytes[1] == 123);
        CHECK(sendMidi[1].rawBytes[2] == 0);
      }
    }
  }
}

TEST_CASE("When Send Channels change Note off gets send on previous Channels", "[MIDI][TCD]")
{
  std::vector<nltools::msg::Midi::SimpleMessage> sendMidi;
  std::vector<MidiChannelModeMessages> messages;
  MockDSPHost host;
  MidiRuntimeOptions options;
  InputEventStage eventStage(&host, &options, {}, [&](auto m){ sendMidi.emplace_back(m); }, [&](auto c){ messages.emplace_back(c); });

  auto msg = nltools::msg::Setting::MidiSettingsMessage{};
  msg.routings = TestHelper::createFullMappings(true);
  msg.sendChannel = MidiSendChannel::CH_1;
  msg.sendSplitChannel = MidiSendChannelSplit::CH_2;
  options.update(msg);
  auto old = options.getLastReceivedMessage();

  WHEN("Previous Notes Enable is disabled")
  {
    msg.sendChannel = MidiSendChannel::CH_3;
    msg.sendSplitChannel = MidiSendChannelSplit::CH_4;
    options.update(msg);

    WHEN("DSPHost is Single")
    {
      host.setType(SoundType::Single);
      eventStage.onMidiSettingsMessageWasReceived(msg, old);

      THEN("All Notes Off was send")
      {
        REQUIRE(sendMidi.size() == 1);
        auto channel = sendMidi[0].rawBytes[0] & 0b00001111;
        CHECK(channel == 0);
        CHECK(sendMidi[0].rawBytes[1] == 123);
        CHECK(sendMidi[0].rawBytes[2] == 0);
      }
    }

    WHEN("DSPHost is Split")
    {
      host.setType(SoundType::Split);
      eventStage.onMidiSettingsMessageWasReceived(msg, old);

      THEN("All Notes Off was send")
      {
        REQUIRE(sendMidi.size() == 2);
        auto channel = sendMidi[0].rawBytes[0] & 0b00001111;
        CHECK(channel == 0);
        CHECK(sendMidi[0].rawBytes[1] == 123);
        CHECK(sendMidi[0].rawBytes[2] == 0);
        auto channelSec = sendMidi[1].rawBytes[0] & 0b00001111;
        CHECK(channelSec == 1);
        CHECK(sendMidi[1].rawBytes[1] == 123);
        CHECK(sendMidi[1].rawBytes[2] == 0);
      }
    }
  }
}

TEST_CASE("When Receive Channels change DSP->Flush should get called", "[MIDI][TCD]")
{
  std::vector<nltools::msg::Midi::SimpleMessage> sendMidi;
  std::vector<MidiChannelModeMessages> messages;
  MockDSPHost host;
  MidiRuntimeOptions options;
  InputEventStage eventStage(&host, &options, {}, [&](auto m){ sendMidi.emplace_back(m); }, [&](auto c){ messages.emplace_back(c); });

  auto msg = nltools::msg::Setting::MidiSettingsMessage{};
  msg.routings = TestHelper::createFullMappings(true);
  msg.sendChannel = MidiSendChannel::CH_1;
  msg.sendSplitChannel = MidiSendChannelSplit::CH_2;
  msg.receiveChannel = MidiReceiveChannel::CH_10;
  msg.receiveSplitChannel = MidiReceiveChannelSplit::CH_11;
  options.update(msg);
  auto old = options.getLastReceivedMessage();

  WHEN("ReceiveChannels changed")
  {
    msg.receiveChannel = MidiReceiveChannel::CH_3;
    msg.receiveSplitChannel = MidiReceiveChannelSplit::CH_4;
    options.update(msg);

    WHEN("DSPHost is Single")
    {
      host.setType(SoundType::Single);
      eventStage.onMidiSettingsMessageWasReceived(msg, old);

      THEN("No Midi was send")
      {
        REQUIRE(sendMidi.empty());
      }

      THEN("DSP host flush scheduled")
      {
        REQUIRE(messages.size() == 1);
        CHECK(messages[0] == AllSoundOff);
      }
    }

    WHEN("DSPHost is Split")
    {
      host.setType(SoundType::Split);
      eventStage.onMidiSettingsMessageWasReceived(msg, old);

      THEN("No Midi was send")
      {
        REQUIRE(sendMidi.empty());
      }

      THEN("DSP host flush scheduled only once")
      {
        REQUIRE(messages.size() == 1);
        CHECK(messages[0] == AllSoundOff);
      }
    }
  }
}