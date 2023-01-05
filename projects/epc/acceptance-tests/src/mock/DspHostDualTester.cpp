#include "DspHostDualTester.h"
#include <synth/c15-audio-engine/dsp_host_dual.h>
#include <ParameterId.h>

DspHostDualTester::DspHostDualTester(dsp_host_dual* _host)
    : m_host { _host }
{
}

unsigned int DspHostDualTester::getActiveVoices(const VoiceGroup _group)
{
  // retrieving the polyphonic Gate Envelope signal from both Poly Sections/Parts (12 Voices per Part)
  // (per Voice: Gate Signal is 1.0 (immediately) after KeyPress, and (almost immediately) 0.0 after KeyRelease)

  const auto gateSignalIndex = static_cast<uint32_t>(C15::Signals::Truepoly_Signals::Env_G_Sig);
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
    case VoiceGroup::Invalid:
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
    default:
      break;
  }
  // should never be reached
  nltools_assertNotReached();
  return 1;
}

void DspHostDualTester::applyMonoMessage(const Polyphony _mono, const VoiceGroup _group)
{
  nltools::msg::PolyphonicUnmodulateableParameterChangedMessage msg;
  msg.m_id = C15::PID::Mono_Grp_Enable;
  msg.m_controlPosition = (double) _mono;
  // overwrite voiceGroup in non-single sounds
  msg.m_voiceGroup = m_host->getType() == SoundType::Single ? VoiceGroup::Global : _group;
  // propagate message
  m_host->onParameterChangedMessage(msg);
}

void DspHostDualTester::applyUnisonMessage(const unsigned int _unison, const VoiceGroup _group)
{
  // prepare default message
  nltools::msg::PolyphonicUnmodulateableParameterChangedMessage msg;
  msg.m_id = C15::PID::Unison_Voices;
  // act according to sound type
  msg.m_controlPosition = m_host->getType() == SoundType::Single
      ? encodeUnisonVoice(_unison, C15::Config::total_polyphony)
      : encodeUnisonVoice(_unison, C15::Config::local_polyphony);
  // overwrite voiceGroup in non-single sounds
  msg.m_voiceGroup = m_host->getType() == SoundType::Single ? VoiceGroup::Global : _group;
  m_host->onParameterChangedMessage(msg);
}

void DspHostDualTester::applyMalformedSinglePreset(const MalformedPresetDescriptor& _preset)
{
  // prepare message
  nltools::msg::SinglePresetMessage msg;
  msg.m_polyphonicUnmodulateables[(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Mono_Grp_Enable]
      .m_controlPosition
      = static_cast<double>(_preset.m_mono);
  msg.m_polyphonicUnmodulateables[(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Unison_Voices]
      .m_controlPosition
      = encodeUnisonVoice(_preset.m_unison, C15::Config::total_polyphony);
  // propagate message
  m_host->onPresetMessage(msg);
}

void DspHostDualTester::applyMalformedSplitPreset(const MalformedPresetDescriptor& _partI,
                                                  const MalformedPresetDescriptor& _partII)
{
  // prepare message
  nltools::msg::SplitPresetMessage msg;
  msg.m_polyphonicUnmodulateables[0][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Mono_Grp_Enable]
      .m_controlPosition
      = static_cast<double>(_partI.m_mono);
  msg.m_polyphonicUnmodulateables[1][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Mono_Grp_Enable]
      .m_controlPosition
      = static_cast<double>(_partII.m_mono);
  msg.m_polyphonicUnmodulateables[0][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Unison_Voices]
      .m_controlPosition
      = encodeUnisonVoice(_partI.m_unison, C15::Config::local_polyphony);
  msg.m_polyphonicUnmodulateables[1][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Unison_Voices]
      .m_controlPosition
      = encodeUnisonVoice(_partII.m_unison, C15::Config::local_polyphony);
  // propagate message
  m_host->onPresetMessage(msg);
}

void DspHostDualTester::applyMalformedLayerPreset(const MalformedPresetDescriptor& _preset)
{
  // prepare message
  nltools::msg::LayerPresetMessage msg;
  const auto mono = static_cast<double>(_preset.m_mono);
  msg.m_polyphonicUnmodulateables[0][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Mono_Grp_Enable]
      .m_controlPosition
      = mono;
  msg.m_polyphonicUnmodulateables[1][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Mono_Grp_Enable]
      .m_controlPosition
      = mono;
  const auto unison = encodeUnisonVoice(_preset.m_unison, C15::Config::local_polyphony);
  msg.m_polyphonicUnmodulateables[0][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Unison_Voices]
      .m_controlPosition
      = unison;
  msg.m_polyphonicUnmodulateables[1][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Unison_Voices]
      .m_controlPosition
      = unison;
  // propagate message
  m_host->onPresetMessage(msg);
}

void DspHostDualTester::applyTCDKeyDown(const unsigned int _pitch, const float _velocity, const VoiceGroup _group)
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
    default:
      break;
  }
}

void DspHostDualTester::applyTCDKeyUp(const unsigned int _pitch, const float _velocity, const VoiceGroup _group)
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
    default:
      break;
  }
}

void DspHostDualTester::applyMidiNoteOn(const unsigned int _pitch, const float _velocity,
                                        const MockInputEventSource _inputSrc, const VoiceGroup _group)
{
  // translate MockInputEventSource to External
  DSPInterface::InputEventSource inputSource;
  switch(_inputSrc)
  {
    case MockInputEventSource::Singular:
      inputSource = DSPInterface::InputEventSource::External_Use_Split;
      break;
    case MockInputEventSource::Primary:
      inputSource = DSPInterface::InputEventSource::External_Primary;
      break;
    case MockInputEventSource::Secondary:
      inputSource = DSPInterface::InputEventSource::External_Secondary;
      break;
    case MockInputEventSource::Both:
      inputSource = DSPInterface::InputEventSource::External_Both;
      break;
  }
  // act according to SoundType
  switch(m_host->getType())
  {
    case SoundType::Single:
    case SoundType::Layer:
      m_host->onKeyDown(_pitch, _velocity, inputSource);
      break;
    case SoundType::Split:
      m_host->onKeyDownSplit(_pitch, _velocity, _group, inputSource);
      break;
    default:
      break;
  }
}

void DspHostDualTester::applyMidiNoteOff(const unsigned int _pitch, const float _velocity,
                                         const MockInputEventSource _inputSrc, const VoiceGroup _group)
{
  // translate MockInputEventSource to External
  DSPInterface::InputEventSource inputSource;
  switch(_inputSrc)
  {
    case MockInputEventSource::Singular:
      inputSource = DSPInterface::InputEventSource::External_Use_Split;
      break;
    case MockInputEventSource::Primary:
      inputSource = DSPInterface::InputEventSource::External_Primary;
      break;
    case MockInputEventSource::Secondary:
      inputSource = DSPInterface::InputEventSource::External_Secondary;
      break;
    case MockInputEventSource::Both:
      inputSource = DSPInterface::InputEventSource::External_Both;
      break;
  }
  // act according to SoundType
  switch(m_host->getType())
  {
    case SoundType::Single:
    case SoundType::Layer:
      m_host->onKeyUp(_pitch, _velocity, inputSource);
      break;
    case SoundType::Split:
      m_host->onKeyUpSplit(_pitch, _velocity, _group, inputSource);
      break;
    default:
      break;
  }
}

bool DspHostDualTester::StereoOutput::isSilence()
{
  return (m_left + m_right) == 0.0f;  // is this really legit?
}

DspHostDualTester::StereoOutput DspHostDualTester::scanOutput(const std::vector<SampleFrame>& _audiodata)
{
  StereoOutput result;
  for(SampleFrame sample : _audiodata)
  {
    result.m_left += std::abs(sample.left);
    result.m_left += std::abs(sample.right);
  }
  return result;
}

double DspHostDualTester::encodeUnisonVoice(const unsigned int _unison, const unsigned int _polyphony)
{
  nltools_assertAlways(_unison > 0 && _unison <= _polyphony);
  return static_cast<double>(_unison - 1) / static_cast<double>(_polyphony - 1);
}

void DspHostDualTester::setSplit(VoiceGroup vg, float pos)
{
  nltools::msg::PolyphonicModulateableParameterChangedMessage msg {};
  msg.m_controlPosition = static_cast<double>(pos);
  msg.m_id = C15::PID::Split_Split_Point;
  msg.m_voiceGroup = vg;
  m_host->onParameterChangedMessage(msg);
}

void DspHostDualTester::applyMalformedSplitPreset(const DspHostDualTester::MalformedPresetDescriptor& _partI,
                                                  const DspHostDualTester::MalformedPresetDescriptor& _partII,
                                                  float split)
{
  nltools::msg::SplitPresetMessage msg;
  msg.m_polyphonicUnmodulateables[0][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Mono_Grp_Enable]
      .m_controlPosition
      = static_cast<double>(_partI.m_mono);
  msg.m_polyphonicUnmodulateables[1][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Mono_Grp_Enable]
      .m_controlPosition
      = static_cast<double>(_partII.m_mono);
  msg.m_polyphonicUnmodulateables[0][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Unison_Voices]
      .m_controlPosition
      = encodeUnisonVoice(_partI.m_unison, C15::Config::local_polyphony);
  msg.m_polyphonicUnmodulateables[0][(uint32_t) C15::Parameters::Polyphonic_Unmodulateables::Unison_Voices]
      .m_controlPosition
      = encodeUnisonVoice(_partII.m_unison, C15::Config::local_polyphony);

  msg.m_polyphonicModulateables[0][(uint32_t) C15::Parameters::Polyphonic_Modulateables::Split_Split_Point]
      .m_controlPosition
      = static_cast<double>(split);
  msg.m_polyphonicModulateables[1][(uint32_t) C15::Parameters::Polyphonic_Modulateables::Split_Split_Point]
      .m_controlPosition
      = static_cast<double>(split) + (1.0 / 61.0);
  // propagate message
  m_host->onPresetMessage(msg);
}
