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
  PassOnKeyDownHost(const int expectedNote, double expectedVelo, VoiceGroup expectedPart)
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
  const double m_vel;
  const VoiceGroup m_part;
};

class PassOnKeyUpHost : public MockDSPHost
{
 public:
  PassOnKeyUpHost(const int expectedNote, double expectedVelo, VoiceGroup expectedPart)
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
  const double m_vel;
  const VoiceGroup m_part;
};

MidiRuntimeOptions createMidiSettings()
{
  MidiRuntimeOptions options;
  nltools::msg::Setting::MidiSettingsMessage msg;
  msg.receiveNotes = true;
  msg.receiveControllers = true;
  msg.receiveProgramChange = true;
  msg.receiveChannel = 16;
  msg.receiveSplitChannel = 16;
  options.update(msg);
  return options;
}

TEST_CASE("Input Event Stage MIDI In KeyDown", "[MIDI]")
{
  PassOnKeyDownHost dsp { 17, 0.5, VoiceGroup::I };

  auto settings = createMidiSettings();
  InputEventStage eventStage(&dsp, &settings, [](nltools::msg::Midi::SimpleMessage msg) { CHECK(false); });

  eventStage.onMIDIMessage({ 0x90, (uint8_t) 17, (uint8_t) 64 });

  CHECK(dsp.didReceiveKeyDown());
}

TEST_CASE("Input Event Stage MIDI In KeyUp", "[MIDI]")
{
  PassOnKeyUpHost dsp { 17, 0.5, VoiceGroup::I };

  auto settings = createMidiSettings();
  InputEventStage eventStage(&dsp, &settings, [](nltools::msg::Midi::SimpleMessage msg) { CHECK(false); });

  eventStage.onMIDIMessage({ 0x80, (uint8_t) 17, (uint8_t) 64 });

  CHECK(dsp.didReceiveKeyUp());
}