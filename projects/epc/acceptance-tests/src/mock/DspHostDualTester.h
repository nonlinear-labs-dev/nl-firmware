#pragma once

#include <synth/c15-audio-engine/dsp_host_dual.h>

class DspHostDualTester
{
public:
  explicit DspHostDualTester(dsp_host_dual* _host);

  // can be used to detect active Voices, for example with VoiceAllocation/Envelope Reset events
  uint8_t getActiveVoices(const VoiceGroup _group = VoiceGroup::Global);

private:
  dsp_host_dual* m_host;
};
