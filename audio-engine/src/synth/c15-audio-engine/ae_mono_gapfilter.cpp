#include "ae_mono_gapfilter.h"

/******************************************************************************/
/** @file       ae_mono_gapfilter.cpp
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      ...
    @todo
*******************************************************************************/

Engine::MonoGapFilter::MonoGapFilter()
{
}

void Engine::MonoGapFilter::init(const float _samplerate)
{
  m_out_L = m_out_R = 0.0f;
  m_warpConst_2PI = NlToolbox::Constants::twopi / _samplerate;
  m_freqClip_max = _samplerate / 2.125f;
  m_freqClip_min = _samplerate / 24576.0f;
  m_freqScale_max = _samplerate / 3.0f;
  m_freqScale_min = _samplerate / 2.0f;
  // biquad highpasses (l,r)
  m_hp_l1_b0 = m_hp_l1_b1 = m_hp_l1_a1 = m_hp_l1_a2 = m_hp_l2_b0 = m_hp_l2_b1 = m_hp_l2_a1 = m_hp_l2_a2
      = m_hp_l1_stateVar_1 = m_hp_l1_stateVar_2 = m_hp_l1_stateVar_3 = m_hp_l1_stateVar_4 = m_hp_l2_stateVar_1
      = m_hp_l2_stateVar_2 = m_hp_l2_stateVar_3 = m_hp_l2_stateVar_4 = 0.0f;
  m_hp_r1_b0 = m_hp_r1_b1 = m_hp_r1_a1 = m_hp_r1_a2 = m_hp_r2_b0 = m_hp_r2_b1 = m_hp_r2_a1 = m_hp_r2_a2
      = m_hp_r1_stateVar_1 = m_hp_r1_stateVar_2 = m_hp_r1_stateVar_3 = m_hp_r1_stateVar_4 = m_hp_r2_stateVar_1
      = m_hp_r2_stateVar_2 = m_hp_r2_stateVar_3 = m_hp_r2_stateVar_4 = 0.0f;
  // biquad lowpasses (l,r)
  m_lp_l1_b0 = m_lp_l1_b1 = m_lp_l1_a1 = m_lp_l1_a2 = m_lp_l2_b0 = m_lp_l2_b1 = m_lp_l2_a1 = m_lp_l2_a2
      = m_lp_l1_stateVar_1 = m_lp_l1_stateVar_2 = m_lp_l1_stateVar_3 = m_lp_l1_stateVar_4 = m_lp_l2_stateVar_1
      = m_lp_l2_stateVar_2 = m_lp_l2_stateVar_3 = m_lp_l2_stateVar_4 = 0.0f;
  m_lp_r1_b0 = m_lp_r1_b1 = m_lp_r1_a1 = m_lp_r1_a2 = m_lp_r2_b0 = m_lp_r2_b1 = m_lp_r2_a1 = m_lp_r2_a2
      = m_lp_r1_stateVar_1 = m_lp_r1_stateVar_2 = m_lp_r1_stateVar_3 = m_lp_r1_stateVar_4 = m_lp_r2_stateVar_1
      = m_lp_r2_stateVar_2 = m_lp_r2_stateVar_3 = m_lp_r2_stateVar_4 = 0.0f;
}

void Engine::MonoGapFilter::set(MonoSignals &_signals)
{
  // temporary variables
  float tmpVar_1, tmpVar_2, frequency, resonance;
  // left channel - biquad highpass
  frequency = _signals.get(C15::Signals::Mono_Signals::Gap_Flt_HF_L);
  tmpVar_1 = (frequency - m_freqScale_min) * (1.0f / (m_freqScale_max - m_freqScale_min));
  tmpVar_1 = std::clamp(frequency, -1.0f, 1.0f);
  resonance = _signals.get(C15::Signals::Mono_Signals::Gap_Flt_Res) * tmpVar_1;
  // left channel - biquad highpass 1
  tmpVar_1 = m_warpConst_2PI * std::clamp(frequency, m_freqClip_min, m_freqClip_max);
  tmpVar_2 = NlToolbox::Math::cos(tmpVar_1);
  m_hp_l1_a1 = tmpVar_2 * -2.0f;
  m_hp_l1_b0 = (1.0f + tmpVar_2) * 0.5f;
  m_hp_l1_b1 = (1.0f + tmpVar_2) * -1.0f;
  tmpVar_2 = NlToolbox::Math::sin(tmpVar_1) * (1.0f - resonance);
  m_hp_l1_a2 = 1.0f - tmpVar_2;
  tmpVar_2 = 1.0f / (1.0f + tmpVar_2);
  m_hp_l1_a1 = m_hp_l1_a1 * -tmpVar_2;
  m_hp_l1_a2 = m_hp_l1_a2 * -tmpVar_2;
  m_hp_l1_b0 = m_hp_l1_b0 * tmpVar_2;
  m_hp_l1_b1 = m_hp_l1_b1 * tmpVar_2;
  // left channel - biquad highpass 2
  frequency *= 0.75f;
  tmpVar_1 = m_warpConst_2PI * std::clamp(frequency, m_freqClip_min, m_freqClip_max);
  tmpVar_2 = NlToolbox::Math::cos(tmpVar_1);
  m_hp_l2_a1 = tmpVar_2 * -2.0f;
  m_hp_l2_b0 = (1.0f + tmpVar_2) * 0.5f;
  m_hp_l2_b1 = (1.0f + tmpVar_2) * -1.0f;
  tmpVar_2 = NlToolbox::Math::sin(tmpVar_1) * (1.0f - resonance);
  m_hp_l2_a2 = 1.0f - tmpVar_2;
  tmpVar_2 = 1.0f / (1.0f + tmpVar_2);
  m_hp_l2_a1 = m_hp_l2_a1 * -tmpVar_2;
  m_hp_l2_a2 = m_hp_l2_a2 * -tmpVar_2;
  m_hp_l2_b0 = m_hp_l2_b0 * tmpVar_2;
  m_hp_l2_b1 = m_hp_l2_b1 * tmpVar_2;
  // right channel - biquad highpass
  frequency = _signals.get(C15::Signals::Mono_Signals::Gap_Flt_HF_R);
  tmpVar_1 = (frequency - m_freqScale_min) * (1.0f / (m_freqScale_max - m_freqScale_min));
  tmpVar_1 = std::clamp(tmpVar_1, -1.0f, 1.0f);
  resonance = _signals.get(C15::Signals::Mono_Signals::Gap_Flt_Res) * tmpVar_1;
  // right channel - biquad highpass 1
  tmpVar_1 = m_warpConst_2PI * std::clamp(frequency, m_freqClip_min, m_freqClip_max);
  tmpVar_2 = NlToolbox::Math::cos(tmpVar_1);
  m_hp_r1_a1 = tmpVar_2 * -2.0f;
  m_hp_r1_b0 = (1.0f + tmpVar_2) * 0.5f;
  m_hp_r1_b1 = (1.0f + tmpVar_2) * -1.0f;
  tmpVar_2 = NlToolbox::Math::sin(tmpVar_1) * (1.0f - resonance);
  m_hp_r1_a2 = 1.0f - tmpVar_2;
  tmpVar_2 = 1.0f / (1.0f + tmpVar_2);
  m_hp_r1_a1 = m_hp_r1_a1 * -tmpVar_2;
  m_hp_r1_a2 = m_hp_r1_a2 * -tmpVar_2;
  m_hp_r1_b0 = m_hp_r1_b0 * tmpVar_2;
  m_hp_r1_b1 = m_hp_r1_b1 * tmpVar_2;
  // right channel - biquad highpass 2
  frequency *= 0.75f;
  tmpVar_1 = m_warpConst_2PI * std::clamp(frequency, m_freqClip_min, m_freqClip_max);
  tmpVar_2 = NlToolbox::Math::cos(tmpVar_1);
  m_hp_r2_a1 = tmpVar_2 * -2.0f;
  m_hp_r2_b0 = (1.0f + tmpVar_2) * 0.5f;
  m_hp_r2_b1 = (1.0f + tmpVar_2) * -1.0f;
  tmpVar_2 = NlToolbox::Math::sin(tmpVar_1) * (1.0f - resonance);
  m_hp_r2_a2 = 1.0f - tmpVar_2;
  tmpVar_2 = 1.0f / (1.0f + tmpVar_2);
  m_hp_r2_a1 = m_hp_r2_a1 * -tmpVar_2;
  m_hp_r2_a2 = m_hp_r2_a2 * -tmpVar_2;
  m_hp_r2_b0 = m_hp_r2_b0 * tmpVar_2;
  m_hp_r2_b1 = m_hp_r2_b1 * tmpVar_2;
  // left channel - biquad lowpass
  frequency = _signals.get(C15::Signals::Mono_Signals::Gap_Flt_LF_L);
  tmpVar_1 = (frequency - m_freqScale_min) * (1.0f / (m_freqScale_max - m_freqScale_min));
  tmpVar_1 = std::clamp(tmpVar_1, -1.0f, 1.0f);
  resonance = _signals.get(C15::Signals::Mono_Signals::Gap_Flt_Res) * tmpVar_1;
  // left channel - biquad lowpass 1
  tmpVar_1 = m_warpConst_2PI * std::clamp(frequency, m_freqClip_min, m_freqClip_max);
  tmpVar_2 = NlToolbox::Math::cos(tmpVar_1);
  m_lp_l1_a1 = tmpVar_2 * -2.0f;
  m_lp_l1_b0 = (1.0f - tmpVar_2) * 0.5f;
  m_lp_l1_b1 = 1.0f - tmpVar_2;
  tmpVar_2 = NlToolbox::Math::sin(tmpVar_1) * (1.0f - resonance);
  m_lp_l1_a2 = 1.0f - tmpVar_2;
  tmpVar_2 = 1.0f / (1.0f + tmpVar_2);
  m_lp_l1_a1 = m_lp_l1_a1 * -tmpVar_2;
  m_lp_l1_a2 = m_lp_l1_a2 * -tmpVar_2;
  m_lp_l1_b0 = m_lp_l1_b0 * tmpVar_2;
  m_lp_l1_b1 = m_lp_l1_b1 * tmpVar_2;
  // left channel - biquad lowpass 2
  frequency *= 1.33f;
  tmpVar_1 = m_warpConst_2PI * std::clamp(frequency, m_freqClip_min, m_freqClip_max);
  tmpVar_2 = NlToolbox::Math::cos(tmpVar_1);
  m_lp_l2_a1 = tmpVar_2 * -2.0f;
  m_lp_l2_b0 = (1.0f - tmpVar_2) * 0.5f;
  m_lp_l2_b1 = 1.0f - tmpVar_2;
  tmpVar_2 = NlToolbox::Math::sin(tmpVar_1) * (1.0f - resonance);
  m_lp_l2_a2 = 1.0f - tmpVar_2;
  tmpVar_2 = 1.0f / (1.0f + tmpVar_2);
  m_lp_l2_a1 = m_lp_l2_a1 * -tmpVar_2;
  m_lp_l2_a2 = m_lp_l2_a2 * -tmpVar_2;
  m_lp_l2_b0 = m_lp_l2_b0 * tmpVar_2;
  m_lp_l2_b1 = m_lp_l2_b1 * tmpVar_2;
  // right channel - biquad lowpass
  frequency = _signals.get(C15::Signals::Mono_Signals::Gap_Flt_LF_R);
  tmpVar_1 = (frequency - m_freqScale_min) * (1.0f / (m_freqScale_max - m_freqScale_min));
  tmpVar_1 = std::clamp(tmpVar_1, -1.0f, 1.0f);
  resonance = _signals.get(C15::Signals::Mono_Signals::Gap_Flt_Res) * tmpVar_1;
  // right channel - biquad lowpass 1
  tmpVar_1 = m_warpConst_2PI * std::clamp(frequency, m_freqClip_min, m_freqClip_max);
  tmpVar_2 = NlToolbox::Math::cos(tmpVar_1);
  m_lp_r1_a1 = tmpVar_2 * -2.0f;
  m_lp_r1_b0 = (1.0f - tmpVar_2) * 0.5f;
  m_lp_r1_b1 = 1.0f - tmpVar_2;
  tmpVar_2 = NlToolbox::Math::sin(tmpVar_1) * (1.0f - resonance);
  m_lp_r1_a2 = 1.0f - tmpVar_2;
  tmpVar_2 = 1.0f / (1.0f + tmpVar_2);
  m_lp_r1_a1 = m_lp_r1_a1 * -tmpVar_2;
  m_lp_r1_a2 = m_lp_r1_a2 * -tmpVar_2;
  m_lp_r1_b0 = m_lp_r1_b0 * tmpVar_2;
  m_lp_r1_b1 = m_lp_r1_b1 * tmpVar_2;
  // right channel - biquad lowpass 2
  frequency *= 1.33f;
  tmpVar_1 = m_warpConst_2PI * std::clamp(frequency, m_freqClip_min, m_freqClip_max);
  tmpVar_2 = NlToolbox::Math::cos(tmpVar_1);
  m_lp_r2_a1 = tmpVar_2 * -2.0f;
  m_lp_r2_b0 = (1.0f - tmpVar_2) * 0.5f;
  m_lp_r2_b1 = 1.0f - tmpVar_2;
  tmpVar_2 = NlToolbox::Math::sin(tmpVar_1) * (1.0f - resonance);
  m_lp_r2_a2 = 1.0f - tmpVar_2;
  tmpVar_2 = 1.0f / (1.0f + tmpVar_2);
  m_lp_r2_a1 = m_lp_r2_a1 * -tmpVar_2;
  m_lp_r2_a2 = m_lp_r2_a2 * -tmpVar_2;
  m_lp_r2_b0 = m_lp_r2_b0 * tmpVar_2;
  m_lp_r2_b1 = m_lp_r2_b1 * tmpVar_2;
}

void Engine::MonoGapFilter::apply(MonoSignals &_signals, const float _rawSample_L, const float _rawSample_R)
{
  // temporary variables
  float tmpVar, hp_sample, lp_sample;
  // left channel - highpass 1
  hp_sample = m_hp_l1_b0 * _rawSample_L;
  hp_sample += m_hp_l1_b1 * m_hp_l1_stateVar_1;
  hp_sample += m_hp_l1_b0 * m_hp_l1_stateVar_2;
  hp_sample += m_hp_l1_a1 * m_hp_l1_stateVar_3;
  hp_sample += m_hp_l1_a2 * m_hp_l1_stateVar_4;
  m_hp_l1_stateVar_2 = m_hp_l1_stateVar_1 + NlToolbox::Constants::DNC_const;
  m_hp_l1_stateVar_1 = _rawSample_L + NlToolbox::Constants::DNC_const;
  m_hp_l1_stateVar_4 = m_hp_l1_stateVar_3 + NlToolbox::Constants::DNC_const;
  m_hp_l1_stateVar_3 = hp_sample + NlToolbox::Constants::DNC_const;
  // left channel - highpass 2
  tmpVar = m_hp_l2_b0 * hp_sample;
  tmpVar += m_hp_l2_b1 * m_hp_l2_stateVar_1;
  tmpVar += m_hp_l2_b0 * m_hp_l2_stateVar_2;
  tmpVar += m_hp_l2_a1 * m_hp_l2_stateVar_3;
  tmpVar += m_hp_l2_a2 * m_hp_l2_stateVar_4;
  m_hp_l2_stateVar_2 = m_hp_l2_stateVar_1 + NlToolbox::Constants::DNC_const;
  m_hp_l2_stateVar_1 = hp_sample + NlToolbox::Constants::DNC_const;
  m_hp_l2_stateVar_4 = m_hp_l2_stateVar_3 + NlToolbox::Constants::DNC_const;
  m_hp_l2_stateVar_3 = tmpVar + NlToolbox::Constants::DNC_const;
  hp_sample = tmpVar;
  // left channel - balance
  tmpVar = hp_sample * _signals.get(C15::Signals::Mono_Signals::Gap_Flt_HP_LP)
      + _rawSample_L * _signals.get(C15::Signals::Mono_Signals::Gap_Flt_In_LP);
  // left channel - lowpass 1
  lp_sample = m_lp_l1_b0 * tmpVar;
  lp_sample += m_lp_l1_b1 * m_lp_l1_stateVar_1;
  lp_sample += m_lp_l1_b0 * m_lp_l1_stateVar_2;
  lp_sample += m_lp_l1_a1 * m_lp_l1_stateVar_3;
  lp_sample += m_lp_l1_a2 * m_lp_l1_stateVar_4;
  m_lp_l1_stateVar_2 = m_lp_l1_stateVar_1 + NlToolbox::Constants::DNC_const;
  m_lp_l1_stateVar_1 = tmpVar + NlToolbox::Constants::DNC_const;
  m_lp_l1_stateVar_4 = m_lp_l1_stateVar_3 + NlToolbox::Constants::DNC_const;
  m_lp_l1_stateVar_3 = lp_sample + NlToolbox::Constants::DNC_const;
  // left channel - lowpass 2
  tmpVar = m_lp_l2_b0 * lp_sample;
  tmpVar += m_lp_l2_b1 * m_lp_l2_stateVar_1;
  tmpVar += m_lp_l2_b0 * m_lp_l2_stateVar_2;
  tmpVar += m_lp_l2_a1 * m_lp_l2_stateVar_3;
  tmpVar += m_lp_l2_a2 * m_lp_l2_stateVar_4;
  m_lp_l2_stateVar_2 = m_lp_l2_stateVar_1 + NlToolbox::Constants::DNC_const;
  m_lp_l2_stateVar_1 = lp_sample + NlToolbox::Constants::DNC_const;
  m_lp_l2_stateVar_4 = m_lp_l2_stateVar_3 + NlToolbox::Constants::DNC_const;
  m_lp_l2_stateVar_3 = tmpVar + NlToolbox::Constants::DNC_const;
  // left channel - output
  lp_sample = tmpVar * _signals.get(C15::Signals::Mono_Signals::Gap_Flt_LP_Out);
  hp_sample *= _signals.get(C15::Signals::Mono_Signals::Gap_Flt_HP_Out);
  m_out_L = hp_sample + lp_sample + (_rawSample_L * _signals.get(C15::Signals::Mono_Signals::Gap_Flt_In_Out));
  // right channel - highpass 1
  hp_sample = m_hp_r1_b0 * _rawSample_R;
  hp_sample += m_hp_r1_b1 * m_hp_r1_stateVar_1;
  hp_sample += m_hp_r1_b0 * m_hp_r1_stateVar_2;
  hp_sample += m_hp_r1_a1 * m_hp_r1_stateVar_3;
  hp_sample += m_hp_r1_a2 * m_hp_r1_stateVar_4;
  m_hp_r1_stateVar_2 = m_hp_r1_stateVar_1 + NlToolbox::Constants::DNC_const;
  m_hp_r1_stateVar_1 = _rawSample_R + NlToolbox::Constants::DNC_const;
  m_hp_r1_stateVar_4 = m_hp_r1_stateVar_3 + NlToolbox::Constants::DNC_const;
  m_hp_r1_stateVar_3 = hp_sample + NlToolbox::Constants::DNC_const;
  // right channel - highpass 2
  tmpVar = m_hp_r2_b0 * hp_sample;
  tmpVar += m_hp_r2_b1 * m_hp_r2_stateVar_1;
  tmpVar += m_hp_r2_b0 * m_hp_r2_stateVar_2;
  tmpVar += m_hp_r2_a1 * m_hp_r2_stateVar_3;
  tmpVar += m_hp_r2_a2 * m_hp_r2_stateVar_4;
  m_hp_r2_stateVar_2 = m_hp_r2_stateVar_1 + NlToolbox::Constants::DNC_const;
  m_hp_r2_stateVar_1 = hp_sample + NlToolbox::Constants::DNC_const;
  m_hp_r2_stateVar_4 = m_hp_r2_stateVar_3 + NlToolbox::Constants::DNC_const;
  m_hp_r2_stateVar_3 = tmpVar + NlToolbox::Constants::DNC_const;
  hp_sample = tmpVar;
  // right channel - balance
  tmpVar = hp_sample * _signals.get(C15::Signals::Mono_Signals::Gap_Flt_HP_LP)
      + _rawSample_R * _signals.get(C15::Signals::Mono_Signals::Gap_Flt_In_LP);
  // right channel - lowpass 1
  lp_sample = m_lp_r1_b0 * tmpVar;
  lp_sample += m_lp_r1_b1 * m_lp_r1_stateVar_1;
  lp_sample += m_lp_r1_b0 * m_lp_r1_stateVar_2;
  lp_sample += m_lp_r1_a1 * m_lp_r1_stateVar_3;
  lp_sample += m_lp_r1_a2 * m_lp_r1_stateVar_4;
  m_lp_r1_stateVar_2 = m_lp_r1_stateVar_1 + NlToolbox::Constants::DNC_const;
  m_lp_r1_stateVar_1 = tmpVar + NlToolbox::Constants::DNC_const;
  m_lp_r1_stateVar_4 = m_lp_r1_stateVar_3 + NlToolbox::Constants::DNC_const;
  m_lp_r1_stateVar_3 = lp_sample + NlToolbox::Constants::DNC_const;
  // right channel - lowpass 2
  tmpVar = m_lp_r2_b0 * lp_sample;
  tmpVar += m_lp_r2_b1 * m_lp_r2_stateVar_1;
  tmpVar += m_lp_r2_b0 * m_lp_r2_stateVar_2;
  tmpVar += m_lp_r2_a1 * m_lp_r2_stateVar_3;
  tmpVar += m_lp_r2_a2 * m_lp_r2_stateVar_4;
  m_lp_r2_stateVar_2 = m_lp_r2_stateVar_1 + NlToolbox::Constants::DNC_const;
  m_lp_r2_stateVar_1 = lp_sample + NlToolbox::Constants::DNC_const;
  m_lp_r2_stateVar_4 = m_lp_r2_stateVar_3 + NlToolbox::Constants::DNC_const;
  m_lp_r2_stateVar_3 = tmpVar + NlToolbox::Constants::DNC_const;
  // right channel - output
  lp_sample = tmpVar * _signals.get(C15::Signals::Mono_Signals::Gap_Flt_LP_Out);
  hp_sample *= _signals.get(C15::Signals::Mono_Signals::Gap_Flt_HP_Out);
  m_out_R = hp_sample + lp_sample + (_rawSample_R * _signals.get(C15::Signals::Mono_Signals::Gap_Flt_In_Out));
}

void Engine::MonoGapFilter::resetDSP()
{
  m_out_L = m_out_R = 0.0f;
  // biquad highpasses (l,r)
  m_hp_l1_stateVar_1 = m_hp_l1_stateVar_2 = m_hp_l1_stateVar_3 = m_hp_l1_stateVar_4 = m_hp_l2_stateVar_1
      = m_hp_l2_stateVar_2 = m_hp_l2_stateVar_3 = m_hp_l2_stateVar_4 = m_hp_r1_stateVar_1 = m_hp_r1_stateVar_2
      = m_hp_r1_stateVar_3 = m_hp_r1_stateVar_4 = m_hp_r2_stateVar_1 = m_hp_r2_stateVar_2 = m_hp_r2_stateVar_3
      = m_hp_r2_stateVar_4 = 0.0f;
  // biquad lowpasses (l,r)
  m_lp_l1_stateVar_1 = m_lp_l1_stateVar_2 = m_lp_l1_stateVar_3 = m_lp_l1_stateVar_4 = m_lp_l2_stateVar_1
      = m_lp_l2_stateVar_2 = m_lp_l2_stateVar_3 = m_lp_l2_stateVar_4 = m_lp_r1_stateVar_1 = m_lp_r1_stateVar_2
      = m_lp_r1_stateVar_3 = m_lp_r1_stateVar_4 = m_lp_r2_stateVar_1 = m_lp_r2_stateVar_2 = m_lp_r2_stateVar_3
      = m_lp_r2_stateVar_4 = 0.0f;
}
