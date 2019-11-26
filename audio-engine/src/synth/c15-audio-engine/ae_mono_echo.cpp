#include "ae_mono_echo.h"

/******************************************************************************/
/** @file       ae_mono_echo.cpp
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      ...
    @todo
*******************************************************************************/

Engine::MonoEcho::MonoEcho()
{
}

void Engine::MonoEcho::init(const float _samplerate, const uint32_t _upsampleFactor)
{
  m_out_L = m_out_R = 0.0f;
  m_warpConst_PI = NlToolbox::Constants::pi / _samplerate;
  m_freqClip_min = _samplerate / 24576.0f;
  m_freqClip_max = _samplerate / 2.125f;
  // 1 pole highpass
  float omega = NlToolbox::Math::tan(50.0f * m_warpConst_PI);
  m_hp_a1 = (1.0f - omega) / (1.0f + omega);
  m_hp_b0 = 1.0f / (1.0f + omega);
  m_hp_b1 = -1.0f / (1.0f + omega);
  m_hp_stateVar_L1 = m_hp_stateVar_L2 = m_hp_stateVar_R1 = m_hp_stateVar_R2 = 0.0f;
  // 1 pole  lowpass
  m_lp_b0 = m_lp_b1 = m_lp_a1 = m_lp_stateVar_L1 = m_lp_stateVar_L2 = m_lp_stateVar_R1 = m_lp_stateVar_R2 = 0.0f;
  // 2 Hz lowpass
  m_lp2hz_b0 = 2.0f * (NlToolbox::Constants::twopi / _samplerate);
  m_lp2hz_b0 = std::min(m_lp2hz_b0, 1.9f);
  m_lp2hz_stateVar_L = m_lp2hz_stateVar_R = 0.0f;
  // delay buffer
  m_stateVar_L = m_stateVar_R = 0.0f;
  m_buffer_indx = 0;
  const uint32_t size = ECHO_BUFFER_SIZE * _upsampleFactor;
  m_buffer_sz_m1 = size - 1;
  m_buffer_L.resize(size);
  std::fill(m_buffer_L.begin(), m_buffer_L.end(), 0.0f);
  m_buffer_R.resize(size);
  std::fill(m_buffer_R.begin(), m_buffer_R.end(), 0.0f);
}

void Engine::MonoEcho::set(MonoSignals &_signals)
{
  float omega = std::clamp(_signals.get(C15::Signals::Mono_Signals::Echo_LPF), m_freqClip_min, m_freqClip_max);
  omega = NlToolbox::Math::tan(omega * m_warpConst_PI);
  m_lp_a1 = (1.0f - omega) / (1.0f + omega);
  m_lp_b0 = omega / (1.0f + omega);
  m_lp_b1 = omega / (1.0f + omega);
}

void Engine::MonoEcho::apply(MonoSignals &_signals, const float _rawSample_L, const float _rawSample_R)
{
  // temporary variables
  float tmpVar;
  int32_t ind_tm1, ind_t0, ind_tp1, ind_tp2;
  // left channel
  tmpVar = (_rawSample_L * _signals.get(C15::Signals::Mono_Signals::Echo_Send))
      + (m_stateVar_L * _signals.get(C15::Signals::Mono_Signals::Echo_FB_Local))
      + (m_stateVar_R * _signals.get(C15::Signals::Mono_Signals::Echo_FB_Cross));
  m_buffer_L[m_buffer_indx] = tmpVar;
  // left channel - 2 Hz lowpass
  tmpVar = _signals.get(C15::Signals::Mono_Signals::Echo_Time_L) - m_lp2hz_stateVar_L;
  tmpVar = tmpVar * m_lp2hz_b0 + m_lp2hz_stateVar_L;
  m_lp2hz_stateVar_L = tmpVar + NlToolbox::Constants::DNC_const;
  // left channel - delay time
  ind_t0 = static_cast<int32_t>(std::round(tmpVar - 0.5f));
  tmpVar = tmpVar - static_cast<float>(ind_t0);
  ind_tm1 = std::max(ind_t0 - 1, 0);
  ind_tp1 = ind_t0 + 1;
  ind_tp2 = ind_t0 + 2;
  ind_tm1 = (m_buffer_indx - ind_tm1) & m_buffer_sz_m1;
  ind_t0 = (m_buffer_indx - ind_t0) & m_buffer_sz_m1;
  ind_tp1 = (m_buffer_indx - ind_tp1) & m_buffer_sz_m1;
  ind_tp2 = (m_buffer_indx - ind_tp2) & m_buffer_sz_m1;
  // left channel - interpolation
  tmpVar = NlToolbox::Math::interpolRT(tmpVar, m_buffer_L[ind_tm1], m_buffer_L[ind_t0], m_buffer_L[ind_tp1],
                                       m_buffer_L[ind_tp2]);
  // left channel - 1 pole lowpass
  m_out_L = m_lp_b0 * tmpVar;
  m_out_L += m_lp_b1 * m_lp_stateVar_L1;
  m_out_L += m_lp_a1 * m_lp_stateVar_L2;
  m_lp_stateVar_L1 = tmpVar + NlToolbox::Constants::DNC_const;
  m_lp_stateVar_L2 = m_out_L + NlToolbox::Constants::DNC_const;
  // left channel - 1 pole highpass
  m_stateVar_L = m_hp_b0 * m_out_L;
  m_stateVar_L += m_hp_b1 * m_hp_stateVar_L1;
  m_stateVar_L += m_hp_a1 * m_hp_stateVar_L2;
  m_hp_stateVar_L1 = m_out_L + NlToolbox::Constants::DNC_const;
  m_hp_stateVar_L2 = m_stateVar_L + NlToolbox::Constants::DNC_const;
  m_stateVar_L += NlToolbox::Constants::DNC_const;
  // left channel - output
  m_out_L = NlToolbox::Crossfades::crossFade(_rawSample_L, m_out_L, _signals.get(C15::Signals::Mono_Signals::Echo_Dry),
                                             _signals.get(C15::Signals::Mono_Signals::Echo_Wet));
  // right channel
  tmpVar = (_rawSample_R * _signals.get(C15::Signals::Mono_Signals::Echo_Send))
      + (m_stateVar_R * _signals.get(C15::Signals::Mono_Signals::Echo_FB_Local))
      + (m_stateVar_L * _signals.get(C15::Signals::Mono_Signals::Echo_FB_Cross));
  m_buffer_R[m_buffer_indx] = tmpVar;
  // right channel - 2 Hz lowpass
  tmpVar = _signals.get(C15::Signals::Mono_Signals::Echo_Time_R) - m_lp2hz_stateVar_R;
  tmpVar = tmpVar * m_lp2hz_b0 + m_lp2hz_stateVar_R;
  m_lp2hz_stateVar_R = tmpVar + NlToolbox::Constants::DNC_const;
  // right channel - delay time
  ind_t0 = static_cast<int32_t>(std::round(tmpVar - 0.5f));
  tmpVar = tmpVar - static_cast<float>(ind_t0);
  ind_tm1 = std::max(ind_t0 - 1, 0);
  ind_tp1 = ind_t0 + 1;
  ind_tp2 = ind_t0 + 2;
  ind_tm1 = (m_buffer_indx - ind_tm1) & m_buffer_sz_m1;
  ind_t0 = (m_buffer_indx - ind_t0) & m_buffer_sz_m1;
  ind_tp1 = (m_buffer_indx - ind_tp1) & m_buffer_sz_m1;
  ind_tp2 = (m_buffer_indx - ind_tp2) & m_buffer_sz_m1;
  // right channel - interpolation
  tmpVar = NlToolbox::Math::interpolRT(tmpVar, m_buffer_R[ind_tm1], m_buffer_R[ind_t0], m_buffer_R[ind_tp1],
                                       m_buffer_R[ind_tp2]);
  // right channel - 1 pole lowpass
  m_out_R = m_lp_b0 * tmpVar;
  m_out_R += m_lp_b1 * m_lp_stateVar_R1;
  m_out_R += m_lp_a1 * m_lp_stateVar_R2;
  m_lp_stateVar_R1 = tmpVar + NlToolbox::Constants::DNC_const;
  m_lp_stateVar_R2 = m_out_R + NlToolbox::Constants::DNC_const;
  // right channel - 1 pole highpass
  m_stateVar_R = m_hp_b0 * m_out_R;
  m_stateVar_R += m_hp_b1 * m_hp_stateVar_R1;
  m_stateVar_R += m_hp_a1 * m_hp_stateVar_R2;
  m_hp_stateVar_R1 = m_out_R + NlToolbox::Constants::DNC_const;
  m_hp_stateVar_R2 = m_stateVar_R + NlToolbox::Constants::DNC_const;
  m_stateVar_R += NlToolbox::Constants::DNC_const;
  /// Brauchen wir das wirklich?
  // right channel - output
  m_out_R = NlToolbox::Crossfades::crossFade(_rawSample_R, m_out_R, _signals.get(C15::Signals::Mono_Signals::Echo_Dry),
                                             _signals.get(C15::Signals::Mono_Signals::Echo_Wet));
  // increase delay buffer index
  m_buffer_indx = (m_buffer_indx + 1) & m_buffer_sz_m1;
}

void Engine::MonoEcho::resetDSP()
{
  m_out_L = m_out_R = 0.0f;
  // 1 pole highpass
  m_hp_stateVar_L1 = m_hp_stateVar_L2 = m_hp_stateVar_R1 = m_hp_stateVar_R2 = 0.0f;
  // 1 pole  lowpass
  m_lp_stateVar_L1 = m_lp_stateVar_L2 = m_lp_stateVar_R1 = m_lp_stateVar_R2 = 0.0f;
  // 2 Hz lowpass
  m_lp2hz_stateVar_L = m_lp2hz_stateVar_R = 0.0f;
  // delay buffer
  m_stateVar_L = m_stateVar_R = 0.0f;
  std::fill(m_buffer_L.begin(), m_buffer_L.end(), 0.0f);
  std::fill(m_buffer_R.begin(), m_buffer_R.end(), 0.0f);
}
