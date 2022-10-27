#pragma once
#include <Types.h>
#include <synth/c15-audio-engine/dsp_host_dual.h>

class DSPInterface;
class MidiRuntimeOptions;

class TCDDecoder
{
 public:
  constexpr static auto INVALID_KEY = -1;

  TCDDecoder(DSPInterface* dsp, MidiRuntimeOptions* options, KeyShift* shift);
  bool decode(const MidiEvent& event);

  [[nodiscard]] DecoderEventType getEventType() const;
  [[nodiscard]] int getKey() const;
  [[nodiscard]] HardwareSource getHardwareSource() const;
  [[nodiscard]] float getValue() const;

  void reset();

 protected:
  int m_key = INVALID_KEY;
  HardwareSource m_hwSource = HardwareSource::NONE;
  float m_value = 0;
  DecoderEventType m_type = DecoderEventType::UNKNOWN;

 private:
  DSPInterface* m_dsp;
  KeyShift* m_keyShift;
  MidiRuntimeOptions* m_options;

  //Constants
  constexpr static const float c_norm_vel = 1.0f / 16383.0f;  // this only applies for velocities from the LPC
  constexpr static const float c_norm_hw = 1.0f / 16000.0f;   // this only applies for hw sources from the LPC
};
