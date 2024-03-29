#include <catch.hpp>
#include <mock/InputEventStageTester.h>
#include <mock/MockDSPHosts.h>
#include <testing/TestHelper.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Pedal Mappings", "[MIDI][TCD]")
{
  constexpr static uint8_t BASE_TCD = 0b11100000;
  constexpr static uint8_t Pedal1 = 0;
  constexpr static auto sixteenThousand = 0b11111010000000;
  constexpr static auto twelveThousand = static_cast<int>(sixteenThousand * 0.75f);
  constexpr static auto fourThousand = static_cast<int>(sixteenThousand * 0.25f);

  constexpr MidiEvent fullPressureTCDEvent
      = { { BASE_TCD | Pedal1, (uint8_t)(sixteenThousand >> 7), (uint8_t)(sixteenThousand & 127) } };
  constexpr MidiEvent threeQuarterPressureTCDEvent
      = { { BASE_TCD | Pedal1, (uint8_t)(twelveThousand >> 7), (uint8_t)(twelveThousand & 127) } };
  constexpr MidiEvent quarterPressureTCDEvent
      = { { BASE_TCD | Pedal1, (uint8_t)(fourThousand >> 7), (uint8_t)(fourThousand & 127) } };
  constexpr MidiEvent noPressureTCDEvent = { { BASE_TCD | Pedal1, 0, 0 } };

  bool receivedHW = false;
  ConfigureableDSPHost host {};
  host.setType(SoundType::Single);
  host.setOnHWChangedCB([&](auto hwID, float hwPos, bool) {
    CHECK(hwID == HardwareSource::PEDAL1);
    CHECK(hwPos == 1.0f);
    receivedHW = true;
  });

  std::vector<nltools::msg::Midi::SimpleMessage> sendMidiMessages;
  MidiRuntimeOptions settings;
  InputEventStage eventStage(
      &host, &settings, [] {}, [&](auto msg) { sendMidiMessages.push_back(msg); }, [](auto) {});

  //set settings to not interfere with CC01
  {
    nltools::msg::Setting::MidiSettingsMessage msg;
    msg.routings = TestHelper::createFullMappings(true);
    msg.receiveChannel = MidiReceiveChannel::CH_1;
    msg.receiveSplitChannel = MidiReceiveChannelSplit::CH_2;

    msg.sendChannel = MidiSendChannel::CH_1;
    msg.sendSplitChannel = MidiSendChannelSplit::None;

    msg.pedal2cc = PedalCC::CC02;
    msg.pedal3cc = PedalCC::CC02;
    msg.pedal4cc = PedalCC::CC02;
    msg.ribbon1cc = RibbonCC::CC02;
    msg.ribbon2cc = RibbonCC::CC02;
    msg.bendercc = BenderCC::CC02;
    msg.aftertouchcc = AftertouchCC::CC02;
    settings.update(msg);
  }

  WHEN("Mapped to Normal CC")
  {
    settings.setPedal1(PedalCC::CC01);

    WHEN("Send CC01, 1.0")
    {
      eventStage.onMIDIMessage({ { 0xB0, 0x01, 127 } });
      CHECK(receivedHW);
    }
  }

  WHEN("Mapped to CC69, Switching")
  {
    settings.setPedal1(PedalCC::CC69);

    WHEN("Send CC01 < 0.5")
    {
      host.setOnHWChangedCB([&](auto hwID, float hwPos, bool) {
        CHECK(hwID == HardwareSource::PEDAL1);
        CHECK(hwPos == 0.0f);
        receivedHW = true;
      });

      eventStage.onMIDIMessage({ { 0xB0, 69, 50 } });
      CHECK(receivedHW);
    }

    WHEN("Send CC01 > 0.5")
    {
      host.setOnHWChangedCB([&](auto hwID, float hwPos, bool) {
        CHECK(hwID == HardwareSource::PEDAL1);
        CHECK(hwPos == 1.0f);
        receivedHW = true;
      });

      eventStage.onMIDIMessage({ { 0xB0, 69, 80 } });
      CHECK(receivedHW);
    }

    WHEN("Send CC01")
    {
      eventStage.onMIDIMessage({ { 0xB0, 1, 127 } });
      CHECK_FALSE(receivedHW);
    }
  }

  WHEN("Pedal 1 And Pedal 2 are mapped to same CC")
  {
    settings.setPedal1(PedalCC::CC01);
    settings.setPedal2(PedalCC::CC01);

    WHEN("Send CC01")
    {
      bool received0 = false;
      bool received1 = false;

      host.setOnHWChangedCB([&](auto hwID, float hwPos, bool) {
        if(hwID == HardwareSource::PEDAL1)
          ;
        received0 = true;
        if(hwID == HardwareSource::PEDAL2)
          received1 = true;

        CHECK(hwPos == 1.0f);
        receivedHW = true;
      });

      eventStage.onMIDIMessage({ { 0xB0, 1, 127 } });

      THEN("Pedal 1 and Pedal 2 received same value")
      {
        CHECK(received0);
        CHECK(received1);
      }
    }

    WHEN("Receive TCD")
    {
      eventStage.onTCDMessage(fullPressureTCDEvent);
      REQUIRE(receivedHW);

      REQUIRE(sendMidiMessages.size() == 2);
      CHECK(sendMidiMessages[0].numBytesUsed == 3);  //LSB
      CHECK(sendMidiMessages[0].rawBytes[0] == 0xB0);
      CHECK(sendMidiMessages[0].rawBytes[1] == 33);
      CHECK(sendMidiMessages[0].rawBytes[2] == 0);

      CHECK(sendMidiMessages[1].numBytesUsed == 3);  //MSB
      CHECK(sendMidiMessages[1].rawBytes[0] == 0xB0);
      CHECK(sendMidiMessages[1].rawBytes[1] == 1);
      CHECK(sendMidiMessages[1].rawBytes[2] == 127);
    }
  }

  WHEN("Pedal 1 is mapped to switching CC and Local Disabled")
  {
    sendMidiMessages.clear();

    {
      auto lastMsg = settings.getLastReceivedMessage();
      lastMsg.pedal1cc = PedalCC::CC69;
      lastMsg.pedal2cc = PedalCC::CC66;
      lastMsg.pedal3cc = PedalCC::CC65;
      lastMsg.pedal4cc = PedalCC::CC64;
      lastMsg.highResCCEnabled = false;
      TestHelper::updateMappingForHW(lastMsg.routings, nltools::msg::Setting::MidiSettingsMessage::RoutingIndex::Pedal1,
                                     nltools::msg::Setting::MidiSettingsMessage::RoutingAspect::LOCAL, false);
      settings.update(lastMsg);
    }

    WHEN("send full pressure")
    {
      eventStage.onTCDMessage(fullPressureTCDEvent);

      CHECK(sendMidiMessages.size() == 1);
      CHECK(sendMidiMessages[0].numBytesUsed == 3);
      CHECK(sendMidiMessages[0].rawBytes[0] == 0xB0);
      CHECK(sendMidiMessages[0].rawBytes[1] == 69);
      CHECK(sendMidiMessages[0].rawBytes[2] == 127);
    }

    WHEN("send no pressure")
    {
      eventStage.onTCDMessage(noPressureTCDEvent);

      CHECK(sendMidiMessages.size() == 1);
      CHECK(sendMidiMessages[0].numBytesUsed == 3);
      CHECK(sendMidiMessages[0].rawBytes[0] == 0xB0);
      CHECK(sendMidiMessages[0].rawBytes[1] == 69);
      CHECK(sendMidiMessages[0].rawBytes[2] == 0);
    }

    WHEN("send quarter pressure")
    {
      eventStage.onTCDMessage(quarterPressureTCDEvent);

      CHECK(sendMidiMessages.size() == 1);
      CHECK(sendMidiMessages[0].numBytesUsed == 3);
      CHECK(sendMidiMessages[0].rawBytes[0] == 0xB0);
      CHECK(sendMidiMessages[0].rawBytes[1] == 69);
      CHECK(sendMidiMessages[0].rawBytes[2] == 0);
    }

    WHEN("send 3/4 pressure")
    {
      eventStage.onTCDMessage(threeQuarterPressureTCDEvent);

      CHECK(sendMidiMessages.size() == 1);
      CHECK(sendMidiMessages[0].numBytesUsed == 3);
      CHECK(sendMidiMessages[0].rawBytes[0] == 0xB0);
      CHECK(sendMidiMessages[0].rawBytes[1] == 69);
      CHECK(sendMidiMessages[0].rawBytes[2] == 127);
    }
  }
}