#pragma once
#include <Types.h>

class DSPInterface;
class MidiRuntimeOptions;

class TCDDecoder
{
 public:
  TCDDecoder(DSPInterface* dsp, MidiRuntimeOptions* options);
  bool decode(const MidiEvent& event);

  [[nodiscard]] DecoderEventType getEventType() const;
  [[nodiscard]] int getKeyOrController() const;
  [[nodiscard]] float getValue() const;

  void reset();

 protected:
  int keyOrController = -1;
  float value = 0;
  DecoderEventType m_type = DecoderEventType::UNKNOWN;

 private:
  DSPInterface* m_dsp;
  MidiRuntimeOptions* m_options;

  //Constants
  constexpr static const float c_norm_vel = 1.0f / 16383.0f;
};