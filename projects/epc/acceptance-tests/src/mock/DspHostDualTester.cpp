#include "DspHostDualTester.h"
#include <synth/c15-audio-engine/dsp_host_dual.h>

DspHostDualTester::DspHostDualTester(dsp_host_dual* _host)
    : m_host{ _host }
{
}

unsigned int DspHostDualTester::getActiveVoices(const VoiceGroup _group)
{
  // retrieving the polyphonic Gate Envelope signal from both Poly Sections/Parts (12 Voices per Part)
  // (per Voice: Gate Signal is 1.0 (immediately) after KeyPress, and (almost immediately) 0.0 after KeyRelease)

  const uint32_t gateSignalIndex = static_cast<uint32_t>(C15::Signals::Truepoly_Signals::Env_G_Sig);
  float gateSignal;

  // obtain specific Part if desired, otherwise both Parts
  switch(_group)
  {
    case VoiceGroup::I:
      gateSignal = sumUp(m_host->m_poly[0].m_signals.get_poly(gateSignalIndex));
      break;
    case VoiceGroup::II:
      gateSignal = sumUp(m_host->m_poly[1].m_signals.get_poly(gateSignalIndex));
      break;
    default:
      gateSignal = sumUp(m_host->m_poly[0].m_signals.get_poly(gateSignalIndex))
          + sumUp(m_host->m_poly[1].m_signals.get_poly(gateSignalIndex));
      break;
  }

  // having summed up all voices, we obtain the number of "active" Voices (0 ... 24)
  // the obtained value should be sample-precise and is not dependant from Sustain or Level Parameters

  return static_cast<uint8_t>(gateSignal);  // 0 : no voice active / 1, 2, ... 23, 24 : n voices active
}

unsigned int DspHostDualTester::getAssignableVoices()
{
  switch(m_host->getType())
  {
    case SoundType::Single:
      return C15::Config::total_polyphony;
    case SoundType::Split:
    case SoundType::Layer:
      return C15::Config::local_polyphony;
  }
  // should never be reached
  return 1;
}

void DspHostDualTester::sendMonoMessage(const bool _mono, const VoiceGroup _group)
{
  // prepare default message
  nltools::msg::UnmodulateableParameterChangedMessage msg{ C15::PID::Mono_Grp_Enable, static_cast<float>(_mono),
                                                           VoiceGroup::Global };
  // overwrite voiceGroup in non-single sounds
  switch(m_host->getType())
  {
    case SoundType::Split:
    case SoundType::Layer:
      msg.voiceGroup = _group;
      break;
  }
  // propagate message
  m_host->localMonoEnableChg(msg);
}

void DspHostDualTester::sendUnisonMessage(const unsigned int _unison, const VoiceGroup _group)
{
  // prepare default message
  nltools::msg::UnmodulateableParameterChangedMessage msg{ C15::PID::Unison_Voices, 0.0f, VoiceGroup::Global };
  // act according to sound type
  switch(m_host->getType())
  {
    case SoundType::Single:
      msg.controlPosition = encodeUnisonVoice(_unison, C15::Config::total_polyphony);
      break;
    case SoundType::Split:
    case SoundType::Layer:
      msg.controlPosition = encodeUnisonVoice(_unison, C15::Config::local_polyphony);
      msg.voiceGroup = _group;
  }
  // propagate message
  m_host->localUnisonVoicesChg(msg);
}

void DspHostDualTester::sendSinglePreset(const bool _mono, const unsigned int _unison)
{
  // prepare message
  nltools::msg::SinglePresetMessage msg;
  msg.mono.monoEnable.controlPosition = static_cast<float>(_mono);
  msg.unison.unisonVoices.controlPosition = encodeUnisonVoice(_unison, C15::Config::total_polyphony);
  // propagate message
  m_host->onPresetMessage(msg);
}

void DspHostDualTester::sendSplitPreset(const bool _monoI, const bool _monoII, const unsigned int _unisonI,
                                        const unsigned int _unisonII)
{
  // prepare message
  nltools::msg::SplitPresetMessage msg;
  msg.mono[0].monoEnable.controlPosition = static_cast<float>(_monoI);
  msg.mono[1].monoEnable.controlPosition = static_cast<float>(_monoII);
  msg.unison[0].unisonVoices.controlPosition = encodeUnisonVoice(_unisonI, C15::Config::local_polyphony);
  msg.unison[1].unisonVoices.controlPosition = encodeUnisonVoice(_unisonII, C15::Config::local_polyphony);
  // propagate message
  m_host->onPresetMessage(msg);
}

void DspHostDualTester::sendLayerPreset(const bool _mono, const unsigned int _unison)
{
  // prepare message
  nltools::msg::LayerPresetMessage msg;
  msg.mono.monoEnable.controlPosition = static_cast<float>(_mono);
  msg.unison.unisonVoices.controlPosition = encodeUnisonVoice(_unison, C15::Config::local_polyphony);
  // propagate message
  m_host->onPresetMessage(msg);
}

void DspHostDualTester::sendTCDKeyDown(const unsigned int _pitch, const float _velocity, const VoiceGroup _group)
{
  switch(m_host->getType())
  {
    case SoundType::Single:
    case SoundType::Layer:
      m_host->onKeyDown(_pitch, _velocity, DSPInterface::InputEventSource::Internal);
      break;
    case SoundType::Split:
      m_host->onKeyDownSplit(_pitch, _velocity, _group, DSPInterface::InputEventSource::Internal);
      break;
  }
}

void DspHostDualTester::sendTCDKeyUp(const unsigned int _pitch, const float _velocity, const VoiceGroup _group)
{
  switch(m_host->getType())
  {
    case SoundType::Single:
    case SoundType::Layer:
      m_host->onKeyUp(_pitch, _velocity, DSPInterface::InputEventSource::Internal);
      break;
    case SoundType::Split:
      m_host->onKeyUpSplit(_pitch, _velocity, _group, DSPInterface::InputEventSource::Internal);
      break;
  }
}

float DspHostDualTester::encodeUnisonVoice(const unsigned int _unison, const unsigned int _polyphony)
{
  const float numerator = static_cast<float>(_unison > 0 ? (_unison < _polyphony ? _unison - 1 : _polyphony - 1) : 0);
  return numerator / static_cast<float>(_polyphony - 1);
}
