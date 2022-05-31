#include <testing/TestHelper.h>
#include <synth/input/InputEventStage.h>
#include <mock/MockDSPHosts.h>
#include <mock/TCDHelpers.h>

TEST_CASE("Issue 2855")
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

    WHEN("Settings are changed with held note")
    {
      host.setType(SoundType::Split);
      eventStage.onTCDMessage(TCD_HELPER::createKeyPosEvent(16));
      eventStage.onTCDMessage(TCD_HELPER::createKeyDownEvent(124, 12));

      TestHelper::updateMappingForHW(msg.routings, tMSG::RoutingIndex::Notes, tMSG::RoutingAspect::RECEIVE_PRIMARY,
                                     false);
      onSettingsReceived(msg);

      THEN("No Crash happened")
      {
        eventStage.onTCDMessage(TCD_HELPER::createKeyPosEvent(16));
        eventStage.onTCDMessage(TCD_HELPER::createKeyUpEvent(0, 0));
      }
    }
  }
}