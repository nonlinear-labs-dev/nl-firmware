#include <testing/TestHelper.h>
#include <mock/MockDSPHosts.h>
#include <MidiRuntimeOptions.h>
#include <synth/input/InputEventStage.h>
#include <mock/InputEventStageTester.h>
#include <mock/MidiOptionsHelpers.h>
#include <mock/TCDHelpers.h>
#include <device-settings/midi/RoutingSettings.h>

TEST_CASE("'Global Local Enable' will be combined with 'RoutingSetting'")
{
  using namespace TCD_HELPER;

  ConfigureableDSPHost dsp {};
  dsp.setType(SoundType::Split);

  MidiRuntimeOptions options {};
  InputEventStage eS(
      &dsp, &options, []() {}, [&](auto) {}, [](auto) {});

  WHEN("All Routings are On")
  {
    MidiOptionsHelper::configureOptions(&options,
                                        [](MidiOptionsHelper::tMSG& msg)
                                        {
                                          msg.routings = TestHelper::createFullMappings(true);
                                          msg.localEnable = true;
                                        });

    bool didReceive = false;
    dsp.setOnHWChangedCB([&didReceive](auto, auto, auto) { didReceive = true; });

    WHEN("HW Event is Send")
    {
      eS.onTCDMessage(createFullPressureHWEvent(TCD_HW_IDS::Pedal1));
      CHECK(didReceive);
    }

    WHEN("HW Event is ignored when routing local is off")
    {
      MidiOptionsHelper::configureOptions(&options,
                                          [](MidiOptionsHelper::tMSG& msg)
                                          {
                                            msg.routings = TestHelper::createFullMappings(false);
                                            msg.localEnable = true;
                                          });

      eS.onTCDMessage(createFullPressureHWEvent(TCD_HW_IDS::Pedal1));
      CHECK_FALSE(didReceive);
    }

    WHEN("HW Event is ignored when global local is off")
    {
      MidiOptionsHelper::configureOptions(&options,
                                          [](MidiOptionsHelper::tMSG& msg)
                                          {
                                            msg.routings = TestHelper::createFullMappings(true);
                                            msg.localEnable = false;
                                          });

      eS.onTCDMessage(createFullPressureHWEvent(TCD_HW_IDS::Pedal1));
      CHECK_FALSE(didReceive);
    }

    WHEN("HW Event is ignored when global and routing local are off")
    {
      MidiOptionsHelper::configureOptions(&options,
                                          [](MidiOptionsHelper::tMSG& msg)
                                          {
                                            msg.routings = TestHelper::createFullMappings(false);
                                            msg.localEnable = false;
                                          });

      eS.onTCDMessage(createFullPressureHWEvent(TCD_HW_IDS::Pedal1));
      CHECK_FALSE(didReceive);
    }
  }
}

TEST_CASE("Key Events Local enable disable")
{
  using namespace TCD_HELPER;

  ConfigureableDSPHost host;
  host.setType(SoundType::Single);

  MidiRuntimeOptions options;
  std::vector<nltools::msg::Midi::SimpleMessage> sendMidi;
  InputEventStage input { &host, &options, []() {}, [&](auto m) { sendMidi.emplace_back(m); }, [](auto) {} };

  WHEN("All routings are on")
  {
    bool received = false;
    host.setOnKeyDownCB([&](int k, float vel, DSPInterface::InputEventSource s) { received = true; });

    WHEN("Global Local is On")
    {
      MidiOptionsHelper::configureOptions(&options,
                                          [](auto& s)
                                          {
                                            s.routings = TestHelper::createFullMappings(true);
                                            s.localEnable = true;
                                          });

      input.onTCDMessage(createKeyPosEvent(12));
      input.onTCDMessage(createKeyDownEvent(127, 127));

      THEN("Note is received")
      {
        CHECK(received);
      }
    }

    WHEN("Global Local is Off")
    {
      MidiOptionsHelper::configureOptions(&options,
                                          [](auto& s)
                                          {
                                            s.routings = TestHelper::createFullMappings(true);
                                            s.localEnable = false;
                                          });

      input.onTCDMessage(createKeyPosEvent(12));
      input.onTCDMessage(createKeyDownEvent(12, 112));

      THEN("Note is not received")
      {
        CHECK_FALSE(received);
      }
    }
  }

  WHEN("All routings are off")
  {
    bool received = false;
    host.setOnKeyDownCB([&](int k, float vel, DSPInterface::InputEventSource s) { received = true; });

    WHEN("Global Local is Off")
    {
      MidiOptionsHelper::configureOptions(&options,
                                          [](auto& s)
                                          {
                                            s.routings = TestHelper::createFullMappings(false);
                                            s.localEnable = false;
                                          });

      input.onTCDMessage(createKeyPosEvent(12));
      input.onTCDMessage(createKeyDownEvent(12, 112));

      THEN("Note is not received")
      {
        CHECK_FALSE(received);
      }
    }

    WHEN("Global Local is On")
    {
      MidiOptionsHelper::configureOptions(&options,
                                          [](auto& s)
                                          {
                                            s.routings = TestHelper::createFullMappings(false);
                                            s.localEnable = true;
                                          });

      input.onTCDMessage(createKeyPosEvent(12));
      input.onTCDMessage(createKeyDownEvent(12, 112));

      THEN("Note is not received")
      {
        CHECK_FALSE(received);
      }
    }
  }
}