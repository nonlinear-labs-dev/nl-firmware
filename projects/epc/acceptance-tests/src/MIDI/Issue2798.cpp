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
  eventStage.onMidiSettingsMessageReceived(msg, false, false);

  WHEN("Previous Notes Enable is disabled")
  {
    msg.routings = TestHelper::createFullMappings(true);
    TestHelper::updateMappings(msg.routings, nltools::msg::Setting::MidiSettingsMessage::RoutingAspect::SEND_PRIMARY, false);
    TestHelper::updateMappings(msg.routings, nltools::msg::Setting::MidiSettingsMessage::RoutingAspect::SEND_SPLIT, false);
    options.update(msg);

    WHEN("DSPHost is Single")
    {
      host.setType(SoundType::Single);
      eventStage.onMidiSettingsMessageReceived(msg, true, true);

      THEN("All Notes Off was send")
      {
        REQUIRE(sendMidi.size() == 1);
        CHECK(sendMidi[0].rawBytes[1] == 123);
        CHECK(sendMidi[0].rawBytes[2] == 0);
      }
    }

    WHEN("DSPHost is Split")
    {
      host.setType(SoundType::Split);
      eventStage.onMidiSettingsMessageReceived(msg, true, true);

      THEN("All Notes Off was send")
      {
        REQUIRE(sendMidi.size() == 2);
        CHECK(sendMidi[0].rawBytes[1] == 123);
        CHECK(sendMidi[0].rawBytes[2] == 0);
        CHECK(sendMidi[1].rawBytes[1] == 123);
        CHECK(sendMidi[1].rawBytes[2] == 0);
      }
    }
  }
}