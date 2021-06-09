#include <testing/TestHelper.h>
#include <mock/MockDSPHosts.h>
#include <mock/InputEventStageTester.h>

TEST_CASE("HW Source Enable Tests")
{
  //Setup
  ConfigureableDSPHost dsp {};
  dsp.setType(SoundType::Split);

  MidiRuntimeOptions options {};

  std::vector<InputEventStage::MIDIOutType> sendMidiMessages;
  InputEventStage eS(
      &dsp, &options,
      []() {

      },
      [&](InputEventStage::MIDIOutType m) { sendMidiMessages.emplace_back(m); });

  InputEventStageTester eventStage(&eS);

  std::vector<std::pair<uint32_t, float>> receivedHWMsg;
  dsp.setOnHWChangedCB([&](auto id, auto pos) { receivedHWMsg.emplace_back(std::make_pair(id, pos)); });

  //prepare default settings

  {
    nltools::msg::Setting::MidiSettingsMessage msg;
    msg.pedal1cc = PedalCC::CC01;
    msg.pedal2cc = PedalCC::CC02;
    msg.pedal3cc = PedalCC::CC03;
    msg.pedal4cc = PedalCC::CC04;
    msg.aftertouchcc = AftertouchCC::CC05;
    msg.bendercc = BenderCC::CC06;
    msg.ribbon1cc = RibbonCC::CC07;
    msg.ribbon2cc = RibbonCC::CC08;

    msg.receiveChannel = MidiReceiveChannel::CH_1;
    msg.receiveSplitChannel = MidiReceiveChannelSplit::CH_2;
    msg.sendChannel = MidiSendChannel::CH_1;
    msg.sendSplitChannel = MidiSendChannelSplit::CH_2;

    msg.highVeloCCEnabled = false;
    msg.highResCCEnabled = false;

    msg.hwMappings = TestHelper::createFullMappings(false);
  }
}