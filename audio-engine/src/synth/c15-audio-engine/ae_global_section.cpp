#include "ae_global_section.h"

/******************************************************************************/
/** @file       ae_global_section.cpp
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new container for all global parameters and dsp
    @todo
*******************************************************************************/

GlobalSection::GlobalSection()
{
  m_out_l = m_out_r = 0.0f;
}

void GlobalSection::init(const float _sampleInc)
{
  m_sampleInc = _sampleInc;
}

void GlobalSection::add_copy_audio_id(const uint32_t _smootherId, const uint32_t _signalId)
{
  m_smoothers.m_copy_audio.add_copy_id(_smootherId, _signalId);
}

void GlobalSection::add_copy_fast_id(const uint32_t _smootherId, const uint32_t _signalId)
{
  m_smoothers.m_copy_fast.add_copy_id(_smootherId, _signalId);
}

void GlobalSection::add_copy_slow_id(const uint32_t _smootherId, const uint32_t _signalId)
{
  m_smoothers.m_copy_slow.add_copy_id(_smootherId, _signalId);
}

void GlobalSection::start_sync(const uint32_t _id, const float _dest)
{
  m_smoothers.start_sync(_id, _dest);
}

void GlobalSection::start_audio(const uint32_t _id, const float _dx, const float _dest)
{
  m_smoothers.start_audio(_id, _dx, _dest);
}

void GlobalSection::start_fast(const uint32_t _id, const float _dx, const float _dest)
{
  m_smoothers.start_fast(_id, _dx, _dest);
}

void GlobalSection::start_slow(const uint32_t _id, const float _dx, const float _dest)
{
  m_smoothers.start_slow(_id, _dx, _dest);
}

float GlobalSection::key_position(const uint32_t _pos)
{
  // provide scaled pitch (without master tune)
  float pos = static_cast<float>(_pos);                // start with pure keyPosition [0 ... 60]
  pos -= static_cast<float>(C15::Config::key_center);  // center to C3 -> 0.0
  pos += evalScale(_pos);                              // add scaling
  return pos;
}

void GlobalSection::render_audio(const float _left0, const float _right0, const float _left1, const float _right1)
{
  // common dsp: smoothers, signal post processing
  m_smoothers.render_audio();
  postProcess_audio();
  // main dsp:
  const float vol = m_smoothers.get(C15::Smoothers::Global_Fast::Master_Volume);
  // (note: master volume seems to not require a signal!)
  // main: test tone
  float phase, squared, signal;
  phase = m_tonePhase - 0.25f;
  phase -= NlToolbox::Conversion::float2int(phase);
  signal = m_toneAmp * NlToolbox::Math::sinP3_noWrap(phase);
  m_tonePhase += m_sampleInc * m_toneFreq;
  m_tonePhase -= NlToolbox::Conversion::float2int(m_tonePhase);
  m_signal[1] = signal;
  // left: master volume, layer combination
  signal = vol * (_left0 + _left1);
  // left: soft clipping
  signal = std::clamp(signal * 0.1588f, -0.25f, 0.25f);
  signal += -0.25f;
  signal += signal;
  signal = 0.5f - std::abs(signal);
  squared = signal * signal;
  signal *= ((((2.26548f * squared) - 5.13274f) * squared) + 3.14159f);
  // left: output signal and test tone combination (mute applied in host)
  m_signal[0] = signal;
  m_signal[2] = signal + m_signal[1];
  m_out_l = m_signal[m_combinationMode];  // 0: C15, 1: Tone, 2: C15 + Tone
  // right: master volume, layer combination
  signal = vol * (_right0 + _right1);
  // right: soft clipping
  signal = std::clamp(signal * 0.1588f, -0.25f, 0.25f);
  signal += -0.25f;
  signal += signal;
  signal = 0.5f - std::abs(signal);
  squared = signal * signal;
  signal *= ((((2.26548f * squared) - 5.13274f) * squared) + 3.14159f);
  // right: output signal and test tone combination (mute applied in host)
  m_signal[0] = signal;
  m_signal[2] = signal + m_signal[1];
  m_out_r = m_signal[m_combinationMode];  // 0: C15, 1: Tone, 2: C15 + Tone
}

void GlobalSection::render_fast()
{
  m_smoothers.render_fast();
  postProcess_fast();
}

void GlobalSection::render_slow()
{
  m_smoothers.render_slow();
  postProcess_slow();
}

void GlobalSection::update_tone_amplitude(const float _db)
{
  m_toneAmp = NlToolbox::Conversion::db2af(_db);
}

void GlobalSection::update_tone_frequency(const float _freq)
{
  m_toneFreq = _freq;
}

void GlobalSection::update_tone_mode(const uint32_t _mode)
{
  m_combinationMode = _mode;
}

void GlobalSection::postProcess_audio()
{
  auto traversal = &m_smoothers.m_copy_audio;
  for(uint32_t i = 0; i < traversal->m_length; i++)
  {
    m_signals.set(traversal->m_signalId[i], m_smoothers.get_audio(traversal->m_smootherId[i]));
  }
}

void GlobalSection::postProcess_fast()
{
  auto traversal = &m_smoothers.m_copy_fast;
  for(uint32_t i = 0; i < traversal->m_length; i++)
  {
    m_signals.set(traversal->m_signalId[i], m_smoothers.get_fast(traversal->m_smootherId[i]));
  }
}

void GlobalSection::postProcess_slow()
{
  auto traversal = &m_smoothers.m_copy_slow;
  for(uint32_t i = 0; i < traversal->m_length; i++)
  {
    m_signals.set(traversal->m_signalId[i], m_smoothers.get_slow(traversal->m_smootherId[i]));
  }
}

float GlobalSection::evalScale(const uint32_t _pos)
{
  // strictly avoiding negative values by octave offset
  int32_t pos = static_cast<int32_t>(_pos + 12);
  // evaluate relative base key
  pos -= static_cast<int32_t>(m_smoothers.get(C15::Smoothers::Global_Sync::Scale_Base_Key));
  // retrieve actual index
  const uint32_t index = static_cast<uint32_t>(pos) % 12;
  // return offset according to index
  if(index == 0)
  {
    // at base key, offset is zero
    return 0.0f;
  }
  else
  {
    // otherwise, offset is found at actual index (+1, ..., +11)
    return m_smoothers.get_sync(index);
  }
}
