#pragma once

#include <nltools/Types.h>
#include <nltools/messaging/Message.h>

class dsp_host_dual;

class DspHostDualTester
{
 public:
  explicit DspHostDualTester(dsp_host_dual* _host);

  // can be used to detect active Voices, for example with VoiceAllocation/Envelope Reset events
  unsigned int getActiveVoices(const VoiceGroup _group = VoiceGroup::Global);

  // todo: voice alloc: get assignable voices,
  unsigned int getAssignableVoices();

  // mono, unison msg generators
  void sendMonoMessage(const bool _mono, const VoiceGroup _group = VoiceGroup::Global);
  void sendUnisonMessage(const unsigned int _unison, const VoiceGroup _group = VoiceGroup::Global);

  // preset generators
  void sendSinglePreset(const bool _mono, const unsigned int _unison);
  void sendSplitPreset(const bool _monoI, const bool _monoII, const unsigned int _unisonI,
                       const unsigned int _unisonII);
  void sendLayerPreset(const bool _mono, const unsigned int _unison);

  // key generators
  void sendTCDKeyDown(const unsigned int _pitch, const float _velocity, const VoiceGroup _group = VoiceGroup::Global);
  void sendTCDKeyUp(const unsigned int _pitch, const float _velocity, const VoiceGroup _group = VoiceGroup::Global);

 private:
  dsp_host_dual* m_host;
  float encodeUnisonVoice(const unsigned int _unison, const unsigned int _polyphony);
};
