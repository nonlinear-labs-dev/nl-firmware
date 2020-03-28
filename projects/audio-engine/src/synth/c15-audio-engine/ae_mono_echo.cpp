#include "ae_mono_echo.h"

/******************************************************************************/
/** @file       ae_mono_echo.cpp
    @date
    @version    1.7-3
    @author     A. Schmied, M. Seeber
    @brief      ...
    @todo
*******************************************************************************/

Engine::MonoEcho::MonoEcho()
{
}

void Engine::MonoEcho::init(const float _samplerate, const uint32_t _upsampleFactor)
{
  // outputs
  m_out_L = 0.0f;
  m_out_R = 0.0f;
  // prewarp
  m_warpConst_PI = NlToolbox::Constants::pi / _samplerate;
  m_freqClip_min = _samplerate / 24576.0f;
  m_freqClip_max = _samplerate / 2.125f;
  // 1p hp
  float omega = NlToolbox::Math::tan(50.0f * m_warpConst_PI);  // 0.000057116 (48 kHz), 0.000028558 (96 kHz)
  float denom = 1.0f / (1.0f + omega);                         // divide once for better performance, then multiply
  m_hp_a1 = (1.0f - omega) * denom;
  m_hp_b0 = denom;
  m_hp_b1 = -denom;
  m_hp_stateVar_L1 = 0.0f;
  m_hp_stateVar_L2 = 0.0f;
  m_hp_stateVar_R1 = 0.0f;
  m_hp_stateVar_R2 = 0.0f;
  // 1p lp
  m_lp_b0 = 0.0f;
  m_lp_b1 = 0.0f;
  m_lp_a1 = 0.0f;
  m_lp_stateVar_L1 = 0.0f;
  m_lp_stateVar_L2 = 0.0f;
  m_lp_stateVar_R1 = 0.0f;
  m_lp_stateVar_R2 = 0.0f;
  // 2 Hz lp
  m_lp2hz_b0 = 2.0f * (NlToolbox::Constants::twopi / _samplerate);
  m_lp2hz_b0 = std::min(m_lp2hz_b0, 1.9f);
  m_lp2hz_stateVar_L = 0.0f;
  m_lp2hz_stateVar_R = 0.0f;
  // delay buffer
  m_stateVar_L = 0.0f;
  m_stateVar_R = 0.0f;
  m_buffer_indx = 0;
  m_buffer_sz_m1 = ECHO_BUFFER_SIZE * _upsampleFactor - 1;
  m_buffer_L.resize(ECHO_BUFFER_SIZE * _upsampleFactor);
  std::fill(m_buffer_L.begin(), m_buffer_L.end(), 0.0f);
  m_buffer_R.resize(ECHO_BUFFER_SIZE * _upsampleFactor);
  std::fill(m_buffer_R.begin(), m_buffer_R.end(), 0.0f);
}

void Engine::MonoEcho::set(MonoSignals &_signals)
{
  float omega = std::clamp(_signals.get(C15::Signals::Mono_Signals::Echo_LPF), m_freqClip_min, m_freqClip_max);
  // omega: clamp([261.625565301 ... 26579.500645116], [1.953125 (48K), 3.90625 (96K)], [22588.235294118 (48K), 45176.470588235 (96K)]
  omega = NlToolbox::Math::tan(omega * m_warpConst_PI);
  // tan([0.017123353 ... 1.478396543]) (48K), tan([0.008561677 ... 0.869812125]) (96K) --> always tan(0 < angle < pi/2), should be okay and > 0
  float denom = 1.0f / (1.0f + omega);  // divide once for better performance, then multiply
  m_lp_a1 = (1.0f - omega) * denom;
  m_lp_b0 = m_lp_b1 = omega * denom;
}

void Engine::MonoEcho::apply(MonoSignals &_signals, const float _rawSample_L, const float _rawSample_R)
{
  float tmpVar;
  // left channel
  tmpVar = (_rawSample_L * _signals.get(C15::Signals::Mono_Signals::Echo_Send))
      + (m_stateVar_L * _signals.get(C15::Signals::Mono_Signals::Echo_FB_Local))
      + (m_stateVar_R * _signals.get(C15::Signals::Mono_Signals::Echo_FB_Cross));
  m_buffer_L[m_buffer_indx] = tmpVar;
  // 2 Hz lp
  tmpVar = _signals.get(C15::Signals::Mono_Signals::Echo_Time_L) - m_lp2hz_stateVar_L;
  tmpVar = tmpVar * m_lp2hz_b0 + m_lp2hz_stateVar_L;
  m_lp2hz_stateVar_L = tmpVar + NlToolbox::Constants::DNC_const;
  int32_t ind_t0 = static_cast<int32_t>(std::round(tmpVar - 0.5f));
  tmpVar = tmpVar - static_cast<float>(ind_t0);
  int32_t ind_tm1 = std::max(ind_t0 - 1, 0);
  int32_t ind_tp1 = ind_t0 + 1;
  int32_t ind_tp2 = ind_t0 + 2;
  ind_tm1 = m_buffer_indx - ind_tm1;
  ind_t0 = m_buffer_indx - ind_t0;
  ind_tp1 = m_buffer_indx - ind_tp1;
  ind_tp2 = m_buffer_indx - ind_tp2;
  ind_tm1 &= m_buffer_sz_m1;
  ind_t0 &= m_buffer_sz_m1;
  ind_tp1 &= m_buffer_sz_m1;
  ind_tp2 &= m_buffer_sz_m1;
  tmpVar = NlToolbox::Math::interpolRT(tmpVar, m_buffer_L[ind_tm1], m_buffer_L[ind_t0], m_buffer_L[ind_tp1],
                                       m_buffer_L[ind_tp2]);
  // lp
  m_out_L = m_lp_b0 * tmpVar;
  m_out_L += m_lp_b1 * m_lp_stateVar_L1;
  m_out_L += m_lp_a1 * m_lp_stateVar_L2;
  m_lp_stateVar_L1 = tmpVar + NlToolbox::Constants::DNC_const;
  m_lp_stateVar_L2 = m_out_L + NlToolbox::Constants::DNC_const;
  // hp
  m_stateVar_L = m_hp_b0 * m_out_L;
  m_stateVar_L += m_hp_b1 * m_hp_stateVar_L1;
  m_stateVar_L += m_hp_a1 * m_hp_stateVar_L2;
  m_hp_stateVar_L1 = m_out_L + NlToolbox::Constants::DNC_const;
  m_hp_stateVar_L2 = m_stateVar_L + NlToolbox::Constants::DNC_const;
  m_stateVar_L += NlToolbox::Constants::DNC_const;
  m_out_L = NlToolbox::Crossfades::crossFade(_rawSample_L, m_out_L, _signals.get(C15::Signals::Mono_Signals::Echo_Dry),
                                             _signals.get(C15::Signals::Mono_Signals::Echo_Wet));
  // right channel
  tmpVar = (_rawSample_R * _signals.get(C15::Signals::Mono_Signals::Echo_Send))
      + (m_stateVar_R * _signals.get(C15::Signals::Mono_Signals::Echo_FB_Local))
      + (m_stateVar_L * _signals.get(C15::Signals::Mono_Signals::Echo_FB_Cross));
  m_buffer_R[m_buffer_indx] = tmpVar;
  // 2 Hz lp
  tmpVar = _signals.get(C15::Signals::Mono_Signals::Echo_Time_R) - m_lp2hz_stateVar_R;
  tmpVar = tmpVar * m_lp2hz_b0 + m_lp2hz_stateVar_R;
  m_lp2hz_stateVar_R = tmpVar + NlToolbox::Constants::DNC_const;
  ind_t0 = static_cast<int32_t>(std::round(tmpVar - 0.5f));
  tmpVar = tmpVar - static_cast<float>(ind_t0);
  ind_tm1 = std::max(ind_t0 - 1, 0);
  ind_tp1 = ind_t0 + 1;
  ind_tp2 = ind_t0 + 2;
  ind_tm1 = m_buffer_indx - ind_tm1;
  ind_t0 = m_buffer_indx - ind_t0;
  ind_tp1 = m_buffer_indx - ind_tp1;
  ind_tp2 = m_buffer_indx - ind_tp2;
  ind_tm1 &= m_buffer_sz_m1;
  ind_t0 &= m_buffer_sz_m1;
  ind_tp1 &= m_buffer_sz_m1;
  ind_tp2 &= m_buffer_sz_m1;
  tmpVar = NlToolbox::Math::interpolRT(tmpVar, m_buffer_R[ind_tm1], m_buffer_R[ind_t0], m_buffer_R[ind_tp1],
                                       m_buffer_R[ind_tp2]);
  // lp
  m_out_R = m_lp_b0 * tmpVar;
  m_out_R += m_lp_b1 * m_lp_stateVar_R1;
  m_out_R += m_lp_a1 * m_lp_stateVar_R2;
  m_lp_stateVar_R1 = tmpVar + NlToolbox::Constants::DNC_const;
  m_lp_stateVar_R2 = m_out_R + NlToolbox::Constants::DNC_const;
  // hp
  m_stateVar_R = m_hp_b0 * m_out_R;
  m_stateVar_R += m_hp_b1 * m_hp_stateVar_R1;
  m_stateVar_R += m_hp_a1 * m_hp_stateVar_R2;
  m_hp_stateVar_R1 = m_out_R + NlToolbox::Constants::DNC_const;
  m_hp_stateVar_R2 = m_stateVar_R + NlToolbox::Constants::DNC_const;
  m_stateVar_R += NlToolbox::Constants::DNC_const;  /// Brauchen wir das wirklich?
  m_out_R = NlToolbox::Crossfades::crossFade(_rawSample_R, m_out_R, _signals.get(C15::Signals::Mono_Signals::Echo_Dry),
                                             _signals.get(C15::Signals::Mono_Signals::Echo_Wet));
  // ringbuffer update
  m_buffer_indx = (m_buffer_indx + 1) & m_buffer_sz_m1;
}

void Engine::MonoEcho::resetDSP()
{
  // outputs
  m_out_L = 0.0f;
  m_out_R = 0.0f;
  // 1p hp
  m_hp_stateVar_L1 = 0.0f;
  m_hp_stateVar_L2 = 0.0f;
  m_hp_stateVar_R1 = 0.0f;
  m_hp_stateVar_R2 = 0.0f;
  // 1p lp
  m_lp_stateVar_L1 = 0.0f;
  m_lp_stateVar_L2 = 0.0f;
  m_lp_stateVar_R1 = 0.0f;
  m_lp_stateVar_R2 = 0.0f;
  // 2 Hz lp
  m_lp2hz_stateVar_L = 0.0f;
  m_lp2hz_stateVar_R = 0.0f;
  // delay buffer
  m_stateVar_L = 0.0f;
  m_stateVar_R = 0.0f;
  std::fill(m_buffer_L.begin(), m_buffer_L.end(), 0.0f);
  std::fill(m_buffer_R.begin(), m_buffer_R.end(), 0.0f);
}
