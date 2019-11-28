#include "ae_poly_comb.h"

/******************************************************************************/
/** @file       ae_poly_comb.cpp
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      comb filter
    @todo
*******************************************************************************/

Engine::PolyCombFilter::PolyCombFilter()
{
}

void Engine::PolyCombFilter::init(const float _samplerate, const uint32_t _upsampleFactor)
{
  m_out = m_decayStateVar = 0.0f;
  m_sampleInterval = 1.f / _samplerate;
  m_warpConst_PI = 3.14159f / _samplerate;
  m_warpConst_2PI = 6.28319f / _samplerate;
  m_freqClip_24576 = _samplerate / 24576.0f;
  m_freqClip_2 = _samplerate / 2.125f;
  m_freqClip_4 = _samplerate / 4.0f;
  m_hpInStateVar = m_hpOutStateVar = 0.0f;
  m_lpStateVar = 0.f;
  m_apStateVar_1 = m_apStateVar_2 = m_apStateVar_3 = m_apStateVar_4 = 0.0f;
  m_buffer_indx = 0;
  m_buffer.resize(COMB_BUFFER_SIZE * _upsampleFactor);
  m_buffer_sz_m1 = (COMB_BUFFER_SIZE * _upsampleFactor) - 1;
  std::fill(m_buffer.begin(), m_buffer.end(), 0.0f);
  m_delayFreqClip = _samplerate / (COMB_BUFFER_SIZE * _upsampleFactor - 2);
  m_delayConst = 0.693147f / (0.0025f * _samplerate);  // 25ms
  m_delaySamples = m_delayStateVar = 0.0f;
}

void Engine::PolyCombFilter::apply(PolySignals &_signals, const PolyValue &_sampleA, const PolyValue &_sampleB)
{
  // ab sample mix
  auto tmpCmbAB = _signals.get(C15::Signals::Quasipoly_Signals::Comb_Flt_In_A_B);
  m_out = (_sampleB * (1.0f - tmpCmbAB)) + (_sampleA * tmpCmbAB);
  // ab sample phase modulation mix
  auto tmpCmbPMAB = _signals.get(C15::Signals::Quasipoly_Signals::Comb_Flt_PM_A_B);
  auto phaseMod = (_sampleA * (1.0f - tmpCmbPMAB)) + (_sampleB * tmpCmbPMAB);
  phaseMod *= _signals.get(C15::Signals::Quasipoly_Signals::Comb_Flt_PM);
  // 1-pole hp
  auto tmpHP = m_hpCoeff_b0 * m_out;
  tmpHP += (m_hpCoeff_b1 * m_hpInStateVar);
  tmpHP += (m_hpCoeff_a1 * m_hpOutStateVar);
  m_hpInStateVar = m_out + NlToolbox::Constants::DNC_const;
  m_hpOutStateVar = tmpHP + NlToolbox::Constants::DNC_const;
  m_out = tmpHP;
  m_out += m_decayStateVar;
  // 1-pole lp
  m_out *= (1.0f - m_lpCoeff);
  m_out += (m_lpCoeff * m_lpStateVar);
  m_out += NlToolbox::Constants::DNC_const;
  m_lpStateVar = m_out;
  // allpass
  auto tmpOut = m_out;
  m_out *= m_apCoeff_2;
  m_out += (m_apStateVar_1 * m_apCoeff_1);
  m_out += m_apStateVar_2;
  m_out -= (m_apStateVar_3 * m_apCoeff_1);
  m_out -= (m_apStateVar_4 * m_apCoeff_2);
  m_out += NlToolbox::Constants::DNC_const;
  m_apStateVar_2 = m_apStateVar_1;
  m_apStateVar_1 = tmpOut;
  m_apStateVar_4 = m_apStateVar_3;
  m_apStateVar_3 = m_out;
  // para d
  for(uint32_t v = 0; v < C15::Config::local_polyphony; v++)
  {
    if(std::abs(m_out[v]) > 0.501187f)
    {
      if(m_out[v] > 0.f)
      {
        m_out[v] -= 0.501187f;
        auto tmpVar = m_out[v];
        m_out[v] = std::min(m_out[v], 2.98815f);
        m_out[v] *= (1.0f - m_out[v] * 0.167328f);
        m_out[v] *= 0.7488f;
        tmpVar *= 0.2512f;
        m_out[v] += (tmpVar + 0.501187f);
      }
      else
      {
        m_out[v] += 0.501187f;
        auto tmpVar = m_out[v];
        m_out[v] = std::max(m_out[v], -2.98815f);
        m_out[v] *= (1.0f - std::abs(m_out[v]) * 0.167328f);
        m_out[v] *= 0.7488f;
        tmpVar *= 0.2512f;
        m_out[v] += (tmpVar - 0.501187f);
      }
    }
  }
  // smooth b
  auto tmpSmooth = m_delaySamples - m_delayStateVar;
  tmpSmooth *= m_delayConst;
  tmpSmooth += m_delayStateVar;
  m_delayStateVar = tmpSmooth;
  tmpSmooth *= _signals.get(C15::Signals::Truepoly_Signals::Comb_Flt_Freq_Env_C);
  tmpSmooth += (phaseMod * tmpSmooth);
  // delay
  auto holdsample = m_out;  // for Bypass
  for(uint32_t v = 0; v < C15::Config::local_polyphony; v++)
  {
    m_buffer[m_buffer_indx][v] = m_out[v];
  }
  /// hier kommt voicestealing hin!!
  tmpSmooth -= 1.0f;
  tmpSmooth = std::clamp(tmpSmooth, 1.0f, 8189.f);
  auto ind_t0 = std::round<int32_t>(tmpSmooth - 0.5f);
  tmpSmooth = tmpSmooth - static_cast<PolyValue>(ind_t0);
  auto ind_tm1 = ind_t0 - 1;
  auto ind_tp1 = ind_t0 + 1;
  auto ind_tp2 = ind_t0 + 2;
  ind_tm1 = m_buffer_indx - ind_tm1;
  ind_t0 = m_buffer_indx - ind_t0;
  ind_tp1 = m_buffer_indx - ind_tp1;
  ind_tp2 = m_buffer_indx - ind_tp2;
  ind_tm1 &= m_buffer_sz_m1;
  ind_t0 &= m_buffer_sz_m1;
  ind_tp1 &= m_buffer_sz_m1;
  ind_tp2 &= m_buffer_sz_m1;
  PolyValue fract, sample_tm1, sample_t0, sample_tp1, sample_tp2;
  fract = tmpSmooth;
  for(uint32_t i = 0; i < C15::Config::local_polyphony; i++)
  {
    sample_tm1[i] = m_buffer[ind_tm1[i]][i];
    sample_t0[i] = m_buffer[ind_t0[i]][i];
    sample_tp1[i] = m_buffer[ind_tp1[i]][i];
    sample_tp2[i] = m_buffer[ind_tp2[i]][i];
  }
  m_out = interpolRT(tmpSmooth, sample_tm1, sample_t0, sample_tp1, sample_tp2);
  /// Envelope for voicestealingtmpVar
  m_buffer_indx = (m_buffer_indx + 1) & m_buffer_sz_m1;
  auto tmpByp = _signals.get(C15::Signals::Truepoly_Signals::Comb_Flt_Bypass);
  m_out = tmpByp * holdsample + (1.f - tmpByp) * m_out;
  // decay
  m_decayStateVar = m_out * m_decayGain;
}

void Engine::PolyCombFilter::set(PolySignals &_signals, const float _samplerate, const uint32_t _voiceId)
{
  // hp coeff
  float frequency = _signals.get(C15::Signals::Truepoly_Signals::Comb_Flt_Freq)[_voiceId];
  frequency *= 0.125f;
  frequency = std::clamp(frequency, m_freqClip_24576, m_freqClip_2);
  frequency *= m_warpConst_PI;
  frequency = NlToolbox::Math::tan(frequency);
  m_hpCoeff_a1[_voiceId] = (1.0f - frequency) / (1.0f + frequency);
  m_hpCoeff_b0[_voiceId] = 1.0f / (1.0f + frequency);
  m_hpCoeff_b1[_voiceId] = m_hpCoeff_b0[_voiceId] * -1.0f;
  // lp coeff
  frequency = _signals.get(C15::Signals::Truepoly_Signals::Comb_Flt_LP_Freq)[_voiceId];
  frequency = std::clamp(frequency, m_freqClip_24576, m_freqClip_4);
  frequency *= m_warpConst_PI;
  frequency *= 0.159155f;  // 2Pi wrap
  frequency -= NlToolbox::Conversion::float2int(frequency);
  frequency *= 6.28319f;
  frequency = NlToolbox::Math::sin(frequency) / NlToolbox::Math::cos(frequency);  // tan -pi..pi
  m_lpCoeff[_voiceId] = (1.0f - frequency) / (1.0f + frequency);
  // ap coeff
  frequency = _signals.get(C15::Signals::Truepoly_Signals::Comb_Flt_AP_Freq)[_voiceId];
  frequency = std::clamp(frequency, m_freqClip_24576, m_freqClip_2);
  frequency *= m_warpConst_2PI;
  float resonance = (_signals.get(C15::Signals::Quasipoly_Signals::Comb_Flt_AP_Res) * 1.99f) - 1.0f;
  resonance = NlToolbox::Math::sin(frequency) * (1.0f - resonance);
  float tmpVar = 1.0f / (1.0f + resonance);
  m_apCoeff_1[_voiceId] = (-2.0f * NlToolbox::Math::cos(frequency)) * tmpVar;
  m_apCoeff_2[_voiceId] = (1.0f - resonance) * tmpVar;
  // delay time
  frequency = _signals.get(C15::Signals::Truepoly_Signals::Comb_Flt_Freq)[_voiceId];
  if(frequency < m_delayFreqClip)
  {
    m_delaySamples[_voiceId] = _samplerate / m_delayFreqClip;
  }
  else
  {
    m_delaySamples[_voiceId] = _samplerate / frequency;
  }
  // lp influence
  frequency *= m_sampleInterval;
  float stateVar_r = NlToolbox::Math::sinP3_wrap(frequency);
  float stateVar_i = NlToolbox::Math::sinP3_wrap(frequency + 0.25f);
  stateVar_r = stateVar_r * m_lpCoeff[_voiceId];
  stateVar_i = stateVar_i * -m_lpCoeff[_voiceId] + 1.0f;
  tmpVar = NlToolbox::Math::arctan(stateVar_r / stateVar_i) * -0.159155f;  // (1.f / -6.28318f)
  m_delaySamples[_voiceId] = m_delaySamples[_voiceId] * tmpVar + m_delaySamples[_voiceId];
  // ap influence
  stateVar_i = NlToolbox::Math::sinP3_wrap(frequency) * -1.0f * m_apCoeff_1[_voiceId];
  stateVar_r = NlToolbox::Math::sinP3_wrap(frequency + 0.25f) * m_apCoeff_1[_voiceId];
  float stateVar2_i = NlToolbox::Math::sinP3_wrap(frequency + frequency);
  float stateVar2_r = NlToolbox::Math::sinP3_wrap(frequency + frequency + 0.25f);
  float var1_i = stateVar_i - stateVar2_i;
  float var2_i = (stateVar_i - (m_apCoeff_2[_voiceId] * stateVar2_i)) * -1.0f;
  float var1_r = stateVar_r + stateVar2_r + m_apCoeff_2[_voiceId];
  float var2_r = stateVar_r + (stateVar2_r * m_apCoeff_2[_voiceId]) + 1.0f;
  stateVar_r = (var1_r * var2_r) - (var1_i * var2_i);
  stateVar_i = (var1_r * var2_i) + (var2_r * var1_i);
  if(stateVar_r > 0.0f)  // safe
  {
    stateVar_r += 1e-12f;
  }
  else
  {
    stateVar_r -= 1e-12f;
  }
  tmpVar = NlToolbox::Math::arctan(stateVar_i / stateVar_r);  // arctan
  if(stateVar_r < 0.0f)
  {
    if(stateVar_i > 0.0f)
    {
      tmpVar += 3.14159f;
    }
    else
    {
      tmpVar -= 3.14159f;
    }
  }
  if(tmpVar > 0.0f)  // forced unwrap > 0
  {
    tmpVar -= 6.28318f;
  }
  tmpVar *= 0.159155f;
  m_delaySamples[_voiceId] = m_delaySamples[_voiceId] * tmpVar + m_delaySamples[_voiceId];
  // decay gain
  tmpVar = _signals.get(C15::Signals::Truepoly_Signals::Comb_Flt_Decay)[_voiceId];
  frequency = _signals.get(C15::Signals::Truepoly_Signals::Comb_Flt_Freq)[_voiceId] * std::abs(tmpVar);
  frequency = std::max(frequency, NlToolbox::Constants::DNC_const);
  frequency = (1.0f / frequency) * -6.28318f;
  if(frequency > 0.0f)  // Exp Clipped
  {
    frequency = 1.0f;
  }
  else if(-27.631f > frequency)
  {
    frequency = 0.0f;
  }
  else
  {
    frequency = std::pow(2.71828f, frequency);
  }
  if(tmpVar < 0.0f)
  {
    frequency *= -1.0f;
  }
  m_decayGain[_voiceId] = frequency;
}

void Engine::PolyCombFilter::setDelaySmoother(const uint32_t _voiceId)
{
  m_delayStateVar[_voiceId] = m_delaySamples[_voiceId];
}

void Engine::PolyCombFilter::resetDSP()
{
  m_out = m_decayStateVar = m_hpInStateVar = m_hpOutStateVar = 0.0f;
  m_lpStateVar = m_apStateVar_1 = m_apStateVar_2 = m_apStateVar_3 = m_apStateVar_4 = 0.0f;
  std::fill(m_buffer.begin(), m_buffer.end(), 0.0f);
  m_delaySamples = m_delayStateVar = 0.0f;
}