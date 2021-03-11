#include <catch.hpp>
#include <synth/input/InputEventStage.h>
#include <synth/C15Synth.h>

class MockDSPHost : public DSPInterface
{
 public:
  void onHWChanged(const uint32_t id, float value) override
  {
  }
  void onKeyDown(const int note, float velocity, VoiceGroup part) override
  {
  }
  void onKeyUp(const int note, float velocity, VoiceGroup part) override
  {
  }
  C15::Properties::HW_Return_Behavior getBehaviour(int id) override
  {
    return C15::Properties::HW_Return_Behavior::Zero;
  }
};

class PassOnKeyDownHost : public MockDSPHost
{
 public:
  PassOnKeyDownHost(const int expectedNote, float expectedVelo, VoiceGroup expectedPart)
      : m_note { expectedNote }
      , m_vel { expectedVelo }
      , m_part { expectedPart }
  {
  }

  void onKeyDown(const int note, float velocity, VoiceGroup part) override
  {
    CHECK(m_note == note);
    CHECK(m_vel == velocity);
    CHECK(m_part == part);
    m_receivedKeyDown = true;
  }

  bool didReceiveKeyDown() const
  {
    return m_receivedKeyDown;
  }

 private:
  bool m_receivedKeyDown = false;
  const int m_note;
  const float m_vel;
  const VoiceGroup m_part;
};

class PassOnKeyUpHost : public MockDSPHost
{
 public:
  PassOnKeyUpHost(const int expectedNote, float expectedVelo, VoiceGroup expectedPart)
      : m_note { expectedNote }
      , m_vel { expectedVelo }
      , m_part { expectedPart }
  {
  }

  void onKeyUp(const int note, float velocity, VoiceGroup part) override
  {
    CHECK(m_note == note);
    CHECK(m_vel == velocity);
    CHECK(m_part == part);
    m_receivedKeyUp = true;
  }

  bool didReceiveKeyUp() const
  {
    return m_receivedKeyUp;
  }

 private:
  bool m_receivedKeyUp = false;
  const int m_note;
  const float m_vel;
  const VoiceGroup m_part;
};

class PassOnHWReceived : public MockDSPHost
{
 public:
  PassOnHWReceived(int expectedId, float expectedValue)
      : m_id { expectedId }
      , m_value { expectedValue }
  {
  }

  void onHWChanged(const uint32_t id, float value) override
  {
    CHECK(m_id == id);
    CHECK(m_value == value);
    m_receivedHW = true;
  }

  [[nodiscard]] bool didReceiveHW() const
  {
    return m_receivedHW;
  }

 private:
  bool m_receivedHW = false;
  const int m_id;
  const float m_value;
};

MidiRuntimeOptions createMidiSettings()
{
  MidiRuntimeOptions options;
  nltools::msg::Setting::MidiSettingsMessage msg;
  msg.receiveNotes = true;
  msg.receiveControllers = true;
  msg.receiveProgramChange = true;
  msg.receiveChannel = MidiReceiveChannel::Omni;
  msg.receiveSplitChannel = MidiReceiveChannelSplit::Omni;

  msg.bendercc = BenderCC::Pitchbend;
  msg.aftertouchcc = AftertouchCC::ChannelPressure;
  msg.pedal1cc = PedalCC::CC20;
  msg.pedal2cc = PedalCC::CC21;
  msg.pedal3cc = PedalCC::CC22;
  msg.pedal4cc = PedalCC::CC23;
  msg.ribbon1cc = RibbonCC::CC24;
  msg.ribbon2cc = RibbonCC::CC25;

  options.update(msg);
  return options;
}

TEST_CASE("Input Event Stage MIDI In KeyDown", "[MIDI]")
{
  PassOnKeyDownHost dsp { 17, 1, VoiceGroup::I };

  auto settings = createMidiSettings();
  InputEventStage eventStage(&dsp, &settings, [](nltools::msg::Midi::SimpleMessage msg) { CHECK(false); });

  WHEN("Send 14 Bit")
  {
    eventStage.onMIDIMessage({ 0xB0, (uint8_t) 88, (uint8_t) 127 });
    eventStage.onMIDIMessage({ 0x90, (uint8_t) 17, (uint8_t) 127 });
    CHECK(dsp.didReceiveKeyDown());
  }

  WHEN("Send 7 Bit")
  {
    eventStage.onMIDIMessage({ 0x90, (uint8_t) 17, (uint8_t) 127 });
    CHECK(dsp.didReceiveKeyDown());
  }
}

TEST_CASE("Input Event Stage MIDI In KeyUp", "[MIDI]")
{
  PassOnKeyUpHost dsp { 17, 1, VoiceGroup::I };

  auto settings = createMidiSettings();
  InputEventStage eventStage(&dsp, &settings, [](nltools::msg::Midi::SimpleMessage msg) { CHECK(false); });

  WHEN("W/o Velo")
  {
    eventStage.onMIDIMessage({ 0x80, (uint8_t) 17, (uint8_t) 127 });

    CHECK(dsp.didReceiveKeyUp());
  }

  WHEN("With Velo")
  {
    eventStage.onMIDIMessage({ 0xB0, 88, 127 });
    eventStage.onMIDIMessage({ 0x80, (uint8_t) 17, (uint8_t) 127 });

    CHECK(dsp.didReceiveKeyUp());
  }
}

TEST_CASE("Input Event Stage MIDI In HWSource -> Pedal1 100%", "[MIDI]")
{
  PassOnHWReceived dsp { 0, 1 };

  auto settings = createMidiSettings();
  InputEventStage eventStage(&dsp, &settings, [](nltools::msg::Midi::SimpleMessage msg) { CHECK(false); });

  WHEN("Send 14 Bit")
  {
    eventStage.onMIDIMessage({ 0xB0, 52, 127 });
    eventStage.onMIDIMessage({ 0xB0, 20, 127 });
    CHECK(dsp.didReceiveHW());
  }

  WHEN("Send 7 Bit")
  {
    eventStage.onMIDIMessage({ 0xB0, 20, 127 });
    CHECK(dsp.didReceiveHW());
  }
}

TEST_CASE("MIDI in of Bender as Channel Pitchbend", "[MIDI]")
{
  PassOnHWReceived dsp { 4, 1 };
  auto settings = createMidiSettings();
  InputEventStage eventStage(&dsp, &settings, [](auto msg) { CHECK(false); });
  eventStage.onMIDIMessage({ 0b11100000, 127, 127 });
  CHECK(dsp.didReceiveHW());
}

TEST_CASE("MIDI in of Bender as Control Change", "[MIDI]")
{
  PassOnHWReceived dsp { 4, 1 };
  auto settings = createMidiSettings();
  settings.setBenderCC(BenderCC::CC01);

  InputEventStage eventStage(&dsp, &settings, [](auto) { CHECK(false); });

  WHEN("Send 14 Bit")
  {
    eventStage.onMIDIMessage({ 0xB0, 33, 127 });
    eventStage.onMIDIMessage({ 0xB0, 1, 127 });
    CHECK(dsp.didReceiveHW());
  }

  WHEN("Send 7 Bit")
  {
    eventStage.onMIDIMessage({ 0xB0, 1, 127 });
    CHECK(dsp.didReceiveHW());
  }
}

TEST_CASE("MIDI in of Aftertouch as Channel Pressure", "[MIDI]")
{
  PassOnHWReceived dsp { 5, 1 };
  auto settings = createMidiSettings();
  settings.setAftertouchCC(AftertouchCC::ChannelPressure);

  InputEventStage eventStage(&dsp, &settings, [](auto) { CHECK(false); });
  eventStage.onMIDIMessage({ 0b11010000, 127, 0 });
  CHECK(dsp.didReceiveHW());
}

TEST_CASE("MIDI in of Aftertouch as Control Change", "[MIDI]")
{
  PassOnHWReceived dsp { 5, 1 };
  auto settings = createMidiSettings();
  settings.setAftertouchCC(AftertouchCC::CC01);

  InputEventStage eventStage(&dsp, &settings, [](auto) { CHECK(false); });

  WHEN("Send 14 Bit")
  {
    eventStage.onMIDIMessage({ 0xB0, 33, 127 });
    eventStage.onMIDIMessage({ 0xB0, 1, 127 });
    CHECK(dsp.didReceiveHW());
  }

  WHEN("Send 7 Bit")
  {
    eventStage.onMIDIMessage({ 0xB0, 1, 127 });
    CHECK(dsp.didReceiveHW());
  }
}