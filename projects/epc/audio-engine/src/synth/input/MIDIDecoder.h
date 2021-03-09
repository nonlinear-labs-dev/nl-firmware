#pragma once
#include <Types.h>
#include <synth/c15-audio-engine/midi_handle.h>
#include <parameter_descriptor.h>
#include <array>

class DSPInterface;

class MIDIDecoder
{
 public:
  MIDIDecoder(DSPInterface* dsp);
  bool decode(const MidiEvent& event);

  [[nodiscard]] DecoderEventType getEventType() const;
  [[nodiscard]] int getKeyOrControl() const;
  [[nodiscard]] float getValue() const;
  [[nodiscard]] int getChannel() const;

 private:
  using CC_Range_7_Bit = Midi::FullCCRange<Midi::Formats::_7_Bits_>;
  using CC_Range_14_Bit = Midi::clipped14BitCCRange;
  using CC_Range_Bender = Midi::FullCCRange<Midi::Formats::_14_Bits_>;
  using CC_Range_Vel = Midi::clipped14BitVelRange;

  std::array<uint8_t, 8> m_hwSourcesMidiLSB;
  uint8_t m_velocityLSB = 0;

  int keyOrControl = -1;
  float value = 0;  // value or velocity
  int m_midiChannel = -1;

  DecoderEventType m_type = DecoderEventType::UNKNOWN;
  DSPInterface* m_dsp;

  void processMidiForHWSource(int id, uint32_t _data);

  template <typename Range> void processBipolarMidiController(const uint32_t dataByte, int id);

  template <typename Range> void processUnipolarMidiController(const uint32_t dataByte, int id);
};