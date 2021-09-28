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
    InputEventStage::OldSettingSnapshot snap(options);
    options.update(msg);
    eventStage.handlePressedNotesOnMidiSettingsChanged(msg, snap);
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
    msg.globalLocalEnable = true;
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

    WHEN("Global Local gets changed")
    {
      msg.globalLocalEnable = false;
      onSettingsReceived(msg);

      THEN("All Notes OFF got send")
      {
        REQUIRE(sendMidi.size() == 2);
        REQUIRE((sendMidi[0].rawBytes[0] & 0b00001111) == 0);
        REQUIRE((sendMidi[1].rawBytes[0] & 0b00001111) == 1);
      }

      THEN("VA and ENV got reset")
      {
        REQUIRE(sendSpecialFuncs.size() == 1);
        CHECK(sendSpecialFuncs[0] == MidiChannelModeMessages::AllSoundOff);
      }
    }
  }
}
