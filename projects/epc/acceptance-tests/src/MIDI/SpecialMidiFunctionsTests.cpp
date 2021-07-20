#include <testing/TestHelper.h>
#include <synth/input/InputEventStage.h>
#include <MidiRuntimeOptions.h>
#include <synth/input/SpecialMidiFunctionInterface.h>
#include <mock/MockDSPHosts.h>

TEST_CASE("Special Midi functions are recognises as such when received via midi")
{

  auto createCCMidiMessage = [](uint8_t channel, uint8_t cc, uint8_t value) {
      return MidiEvent{ static_cast<uint8_t>(0xB0 | channel), cc, value };
  };

  MidiRuntimeOptions options;

  {
    nltools::msg::Setting::MidiSettingsMessage msg;
    msg.hwMappings = TestHelper::createFullMappings(true);
    msg.receiveChannel = MidiReceiveChannel::Omni;
    msg.receiveNotes = true;
    options.update(msg);
  }

  std::function<void(SpecialMidiFunctions)> currentCallback = nullptr;
  ConfigureableDSPHost host;
  InputEventStage foo { &host, &options, []() {}, [](auto) {},
                        [&currentCallback](auto f)
                        {
                          if(currentCallback)
                            currentCallback(f);
                        } };

  WHEN("Panic CC is Send with value 0")
  {
    currentCallback = [](SpecialMidiFunctions f) {
      CHECK(f == SpecialMidiFunctions::AllSoundOff);
    };

    foo.onMIDIMessage(createCCMidiMessage(1, 120, 0));
  }

  WHEN("Panic CC is Send with value =! 0")
  {
    currentCallback = [](SpecialMidiFunctions f) {
        CHECK(f == SpecialMidiFunctions::NOOP);
    };

    foo.onMIDIMessage(createCCMidiMessage(1, 120, 1));
  }
}