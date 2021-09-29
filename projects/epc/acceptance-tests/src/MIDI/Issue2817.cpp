#include <testing/TestHelper.h>
#include <synth/input/InputEventStage.h>
#include <mock/MockDSPHosts.h>

TEST_CASE("Issue 2817")
{
  ConfigureableDSPHost host;
  MidiRuntimeOptions options;
  std::vector<nltools::msg::Midi::SimpleMessage> sendMidi;
  std::vector<MidiChannelModeMessages> sendSpecialFuncs;
  InputEventStage eventStage(
      &host, &options, []() {}, [&](auto midi) { sendMidi.emplace_back(midi); },
      [&](auto specialFunc) { sendSpecialFuncs.emplace_back(specialFunc); });

  auto onSettingsReceived = [&](auto msg)
  {
    auto old = options.getLastReceivedMessage();
    options.update(msg);
    eventStage.onMidiSettingsMessageWasReceived(msg, old);
  };

  WHEN("All Settings On")
  {
    auto msg = options.getLastReceivedMessage();
    msg.routings = TestHelper::createFullMappings(true);
    msg.highResCCEnabled = false;
    msg.highVeloCCEnabled = false;
    msg.sendChannel = MidiSendChannel::CH_1;
    msg.sendSplitChannel = MidiSendChannelSplit::CH_2;
    msg.receiveChannel = MidiReceiveChannel::CH_3;
    msg.receiveSplitChannel = MidiReceiveChannelSplit::CH_4;
    msg.localEnable = true;
    msg.aftertouchcc = AftertouchCC::CC01;
    msg.bendercc = BenderCC::CC02;
    msg.pedal1cc = PedalCC::CC03;
    msg.pedal2cc = PedalCC::CC04;
    msg.pedal3cc = PedalCC::CC05;
    msg.pedal4cc = PedalCC::CC06;
    msg.ribbon1cc = RibbonCC::CC07;
    msg.ribbon2cc = RibbonCC::CC08;
    onSettingsReceived(msg);

    sendSpecialFuncs.clear();
    sendMidi.clear();

    //Receive Notes Disabled -> reset VA and Env
    //Receive Channels changed -> reset VA and Env

    //Send Notes Disabled -> send all Notes off on old channel
    //local notes disabled -> send all notes off on channel
    //Send Channels changed -> send all Notes off on old channel

    using tMSG = decltype(msg);

    WHEN("Receive Primary Notes gets Disabled")
    {
      TestHelper::updateMappingForHW(msg.routings, tMSG::RoutingIndex::Notes, tMSG::RoutingAspect::RECEIVE_PRIMARY,
                                     false);
      onSettingsReceived(msg);

      THEN("VA and ENV got reset")
      {
        REQUIRE(sendSpecialFuncs.size() == 1);
        CHECK(sendSpecialFuncs[0] == MidiChannelModeMessages::AllSoundOff);
      }
    }

    WHEN("Receive Split Notes get Disabled")
    {
      host.setType(SoundType::Split);
      TestHelper::updateMappingForHW(msg.routings, tMSG::RoutingIndex::Notes, tMSG::RoutingAspect::RECEIVE_SPLIT,
                                     false);
      onSettingsReceived(msg);

      THEN("VA and ENV got reset")
      {
        REQUIRE(sendSpecialFuncs.size() == 1);
        CHECK(sendSpecialFuncs[0] == MidiChannelModeMessages::AllSoundOff);
      }
    }

    WHEN("Send Primary Notes get Disabled")
    {
      TestHelper::updateMappingForHW(msg.routings, tMSG::RoutingIndex::Notes, tMSG::RoutingAspect::SEND_PRIMARY, false);
      onSettingsReceived(msg);

      THEN("All Notes OFF got send")
      {
        REQUIRE(sendMidi.size() == 1);
      }

      THEN("VA and ENV not reset")
      {
        REQUIRE(sendSpecialFuncs.empty());
      }
    }

    WHEN("Send Split Notes get Disabled")
    {
      host.setType(SoundType::Split);
      TestHelper::updateMappingForHW(msg.routings, tMSG::RoutingIndex::Notes, tMSG::RoutingAspect::SEND_SPLIT, false);
      onSettingsReceived(msg);

      THEN("All Notes OFF got send")
      {
        REQUIRE(sendMidi.size() == 1);
      }

      THEN("VA and ENV not reset")
      {
        REQUIRE(sendSpecialFuncs.empty());
      }
    }

    WHEN("Send Prim Channel gets changed")
    {
      msg.sendChannel = MidiSendChannel::CH_10;
      onSettingsReceived(msg);

      THEN("All Notes OFF got send")
      {
        REQUIRE(sendMidi.size() == 1);
        REQUIRE((sendMidi[0].rawBytes[0] & 0b00001111) == 0);
      }

      THEN("VA and ENV not reset")
      {
        REQUIRE(sendSpecialFuncs.empty());
      }
    }

    WHEN("Send Split Channel gets changed")
    {
      host.setType(SoundType::Split);
      msg.sendSplitChannel = MidiSendChannelSplit::CH_10;
      onSettingsReceived(msg);

      THEN("All Notes OFF got send")
      {
        REQUIRE(sendMidi.size() == 1);
        REQUIRE((sendMidi[0].rawBytes[0] & 0b00001111) == 1);
      }

      THEN("VA and ENV not reset")
      {
        REQUIRE(sendSpecialFuncs.empty());
      }
    }

    WHEN("Receive Prim Channel gets changed")
    {
      msg.receiveChannel = MidiReceiveChannel::CH_10;
      onSettingsReceived(msg);

      THEN("VA and ENV got reset")
      {
        REQUIRE(sendSpecialFuncs.size() == 1);
        CHECK(sendSpecialFuncs[0] == MidiChannelModeMessages::AllSoundOff);
      }

      THEN("no All Notes off got send")
      {
        REQUIRE(sendMidi.empty());
      }
    }

    WHEN("Receive Split Channel gets changed")
    {
      host.setType(SoundType::Split);
      msg.receiveSplitChannel = MidiReceiveChannelSplit::CH_10;
      onSettingsReceived(msg);

      THEN("VA and ENV got reset")
      {
        REQUIRE(sendSpecialFuncs.size() == 1);
        CHECK(sendSpecialFuncs[0] == MidiChannelModeMessages::AllSoundOff);
      }

      THEN("no All Notes off got send")
      {
        REQUIRE(sendMidi.empty());
      }
    }

    WHEN("Global Local gets changed")
    {
      msg.localEnable = false;
      onSettingsReceived(msg);

      THEN("All Notes OFF got send")
      {
        REQUIRE(sendMidi.empty());
      }

      THEN("VA and ENV got reset")
      {
        REQUIRE(sendSpecialFuncs.size() == 1);
        CHECK(sendSpecialFuncs[0] == MidiChannelModeMessages::AllSoundOff);
      }
    }

    WHEN("HW Source no longer gets send")
    {
      TestHelper::updateMappingForHW(msg.routings, tMSG::RoutingIndex::Bender, tMSG::RoutingAspect::SEND_PRIMARY,
                                     false);

      host.setGetBehaviourCB([](auto hwID) { return C15::Properties::HW_Return_Behavior::Center; });

      onSettingsReceived(msg);

      THEN("Default Position got send via MIDI")
      {
        REQUIRE(sendMidi.size() == 1);
        REQUIRE((sendMidi[0].rawBytes[0] & 0b00001111) == 0);
        auto benderCC = options.getBenderMSBCC().value_or(-1);
        CHECK(benderCC != -1);
        REQUIRE(sendMidi[0].rawBytes[1] == benderCC);
        REQUIRE(sendMidi[0].rawBytes[2] == 64); // 0 bipolar as we are the Bender
      }

      THEN("VA and ENV not reset")
      {
        REQUIRE(sendSpecialFuncs.empty());
      }
    }

    WHEN("HW Source CC gets changed")
    {
      msg.bendercc = BenderCC::CC10;

      host.setType(SoundType::Single);
      host.setGetBehaviourCB([](auto hwID) { return C15::Properties::HW_Return_Behavior::Center; });

      onSettingsReceived(msg);

      THEN("Default Position got send via MIDI on old CC")
      {
        REQUIRE(sendMidi.size() == 1);
        REQUIRE((sendMidi[0].rawBytes[0] & 0b00001111) == 0);
        auto newBenderCC = options.getBenderMSBCC().value_or(-1);
        CHECK(newBenderCC == 10);
        REQUIRE(sendMidi[0].rawBytes[1] != newBenderCC);
        REQUIRE(sendMidi[0].rawBytes[1] == 2);
        REQUIRE(sendMidi[0].rawBytes[2] == 64);
      }

      THEN("VA and ENV not reset")
      {
        REQUIRE(sendSpecialFuncs.empty());
      }
    }
  }
}
