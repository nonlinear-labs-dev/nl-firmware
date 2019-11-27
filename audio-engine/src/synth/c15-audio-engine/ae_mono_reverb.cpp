#include "ae_mono_reverb.h"

/******************************************************************************/
/** @file       ae_mono_reverb.cpp
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      ...
    @todo
*******************************************************************************/

Engine::MonoReverb::MonoReverb()
{
}

void Engine::MonoReverb::init(const float _samplerate, const uint32_t _upsampleFactor)
{
  // output values
  m_out_L = m_out_R = m_out_dry = m_out_wet = 0.0f;
  // inner smoother clock
  m_slow_tick = 0;
  m_slow_thrsh = 2 * _upsampleFactor - 1;
  // reverb modulation
  m_mod_1a = m_mod_2a = m_mod_1b = m_mod_2b = 0.0f;
  m_lfo_omega_1 = 0.86306f * 2.0f / _samplerate;
  m_lfo_omega_2 = 0.6666f * 2.0f / _samplerate;
  // loop filter
  m_warpConst_PI = NlToolbox::Constants::pi / _samplerate;
  m_omegaClip_max = _samplerate / 2.0f;
  m_lp_stateVar_L = m_lp_stateVar_R = m_hp_stateVar_L = m_hp_stateVar_R = 0.0f;
  // delay buffer
  const uint32_t size = REVERB_BUFFER_SIZE * _upsampleFactor;
  m_buffer_indx = 0;
  m_buffer_sz_m1 = size - 1;
  m_buffer_sz_m2 = size - 2;
  // delay buffer - resize left buffers
  m_buffer_L.resize(size);
  m_buffer_L1.resize(size);
  m_buffer_L2.resize(size);
  m_buffer_L3.resize(size);
  m_buffer_L4.resize(size);
  m_buffer_L5.resize(size);
  m_buffer_L6.resize(size);
  m_buffer_L7.resize(size);
  m_buffer_L8.resize(size);
  m_buffer_L9.resize(size);
  // delay buffer - resize right buffers
  m_buffer_R.resize(size);
  m_buffer_R1.resize(size);
  m_buffer_R2.resize(size);
  m_buffer_R3.resize(size);
  m_buffer_R4.resize(size);
  m_buffer_R5.resize(size);
  m_buffer_R6.resize(size);
  m_buffer_R7.resize(size);
  m_buffer_R8.resize(size);
  m_buffer_R9.resize(size);
  // delay buffer - flush left buffers
  std::fill(m_buffer_L.begin(), m_buffer_L.end(), 0.0f);
  std::fill(m_buffer_L1.begin(), m_buffer_L1.end(), 0.0f);
  std::fill(m_buffer_L2.begin(), m_buffer_L2.end(), 0.0f);
  std::fill(m_buffer_L3.begin(), m_buffer_L3.end(), 0.0f);
  std::fill(m_buffer_L4.begin(), m_buffer_L4.end(), 0.0f);
  std::fill(m_buffer_L5.begin(), m_buffer_L5.end(), 0.0f);
  std::fill(m_buffer_L6.begin(), m_buffer_L6.end(), 0.0f);
  std::fill(m_buffer_L7.begin(), m_buffer_L7.end(), 0.0f);
  std::fill(m_buffer_L8.begin(), m_buffer_L8.end(), 0.0f);
  std::fill(m_buffer_L9.begin(), m_buffer_L9.end(), 0.0f);
  // delay buffer - flush right buffers
  std::fill(m_buffer_R.begin(), m_buffer_R.end(), 0.0f);
  std::fill(m_buffer_R1.begin(), m_buffer_R1.end(), 0.0f);
  std::fill(m_buffer_R2.begin(), m_buffer_R2.end(), 0.0f);
  std::fill(m_buffer_R3.begin(), m_buffer_R3.end(), 0.0f);
  std::fill(m_buffer_R4.begin(), m_buffer_R4.end(), 0.0f);
  std::fill(m_buffer_R5.begin(), m_buffer_R5.end(), 0.0f);
  std::fill(m_buffer_R6.begin(), m_buffer_R6.end(), 0.0f);
  std::fill(m_buffer_R7.begin(), m_buffer_R7.end(), 0.0f);
  std::fill(m_buffer_R8.begin(), m_buffer_R8.end(), 0.0f);
  std::fill(m_buffer_R9.begin(), m_buffer_R9.end(), 0.0f);
  // delay buffer - clear state vars
  m_stateVar_L1 = m_stateVar_L2 = m_stateVar_L3 = m_stateVar_L4 = m_stateVar_L5 = m_stateVar_L6 = m_stateVar_L7
      = m_stateVar_L8 = m_stateVar_L9 = m_stateVar_R1 = m_stateVar_R2 = m_stateVar_R3 = m_stateVar_R4 = m_stateVar_R5
      = m_stateVar_R6 = m_stateVar_R7 = m_stateVar_R8 = m_stateVar_R9 = 0.0f;
  // smoothing
  m_smooth_inc = 1.0f / std::max(50.0f * (0.001f * _samplerate / 2.0f), 1.e-12f);  /// 50ms als DEfine Bitte!
}

void Engine::MonoReverb::set(MonoSignals &_signals)
{
  // temporary variables
  float tmpVar, tmp_target;
  // size, chorus
  tmpVar = _signals.get(C15::Signals::Mono_Signals::Reverb_Size);
  tmp_target = _signals.get(C15::Signals::Mono_Signals::Reverb_Chorus) * ((tmpVar * -200.0f) + 311.0f);
  if(m_depth_target - tmp_target != 0.0f)
  {
    m_depth_inc = (m_depth_target - m_depth) * m_smooth_inc;
  }
  tmp_target = tmpVar * ((0.5f - std::abs(tmpVar)) * -0.5f);
  if(m_size_target - tmp_target != 0.0f)
  {
    m_size_target = tmp_target;
    m_size_base = m_size;
    m_size_diff = m_size_target - m_size_base;
    m_size_ramp = 0.0f;
  }
  // balance
  tmp_target = _signals.get(C15::Signals::Mono_Signals::Reverb_Bal);
  if(m_bal_target - tmp_target != 0.0f)
  {
    m_bal_target = tmp_target;
    m_bal_base = m_bal;
    m_bal_diff = m_bal_target - m_bal_base;
    m_bal_ramp = 0.0f;
  }
  // pre delay
  tmpVar = _signals.get(C15::Signals::Mono_Signals::Reverb_Pre);
  tmp_target = std::round(tmpVar);
  if(m_preDel_L_target - tmp_target != 0.0f)
  {
    m_preDel_L_target = tmp_target;
    m_preDel_L_base = m_preDel_L;
    m_preDel_L_diff = m_preDel_L_target - m_preDel_L_base;
    m_preDel_L_ramp = 0.0f;
  }
  tmp_target = std::round(tmpVar * 1.18933f);
  if(m_preDel_R_target - tmp_target != 0.0f)
  {
    m_preDel_R_target = tmp_target;
    m_preDel_R_base = m_preDel_R;
    m_preDel_R_diff = m_preDel_R_target - m_preDel_R_base;
    m_preDel_R_ramp = 0.0f;
  }
  // color - lpf
  tmp_target = std::clamp(_signals.get(C15::Signals::Mono_Signals::Reverb_LPF), 0.1f, m_omegaClip_max);
  tmp_target = NlToolbox::Math::tan(tmp_target * m_warpConst_PI);
  if(m_lp_omega_target - tmp_target != 0.0f)
  {
    m_lp_omega_target = tmp_target;
    m_lp_omega_base = m_lp_omega;
    m_lp_omega_diff = m_lp_omega_target - m_lp_omega_base;
    m_lp_omega_ramp = 0.0f;
  }
  // color - hpf
  tmp_target = std::clamp(_signals.get(C15::Signals::Mono_Signals::Reverb_HPF), 0.1f, m_omegaClip_max);
  tmp_target = NlToolbox::Math::tan(tmp_target * m_warpConst_PI);
  if(m_hp_omega_target - tmp_target != 0.0f)
  {
    m_hp_omega_target = tmp_target;
    m_hp_omega_base = m_hp_omega;
    m_hp_omega_diff = m_hp_omega_target - m_hp_omega_base;
    m_hp_omega_ramp = 0.0f;
  }
}

void Engine::MonoReverb::apply(MonoSignals &_signals, const float _rawSample_L, const float _rawSample_R)
{
  // temporary variables
  float tmpVar, wetSample_L, wetSample_R, wetSample_L2, wetSample_R2;
  int32_t ind_t0, ind_tm1, ind_tp1, ind_tp2;
  // reverb modulation - triggered by internal clock
  if(!m_slow_tick)
  {
    // depth smoother
    if(m_depth_target - m_depth != 0.0f)
    {
      m_depth += m_depth_inc;
    }
    else
    {
      m_depth = m_depth_target;
    }
    // size smoother
    if(m_size_ramp > 1.0f)
    {
      m_size = m_size_target;
    }
    else
    {
      m_size = m_size_base + (m_size_diff * m_size_ramp);
      m_size_ramp += m_smooth_inc;
    }
    m_absorb = m_size * 0.334f + 0.666f;
    m_fb_amnt = m_size * 0.667f + 0.333f;
    // balance smoother
    if(m_bal_ramp > 1.0f)
    {
      m_bal = m_bal_target;
    }
    else
    {
      m_bal = m_bal_base + (m_bal_diff * m_bal_ramp);
      m_bal_ramp += m_smooth_inc;
    }
    tmpVar = m_bal;
    m_bal_full = tmpVar * (2.0f - tmpVar);
    tmpVar = 1.0f - tmpVar;
    m_bal_half = tmpVar * (2.0f - tmpVar);
    // pre delay left smoother
    if(m_preDel_L_ramp > 1.0f)
    {
      m_preDel_L = m_preDel_L_target;
    }
    else
    {
      m_preDel_L = m_preDel_L_base + (m_preDel_L_diff * m_preDel_L_ramp);
      m_preDel_L_ramp += m_smooth_inc;
    }
    // pre delay right smoother
    if(m_preDel_R_ramp > 1.0f)
    {
      m_preDel_R = m_preDel_R_target;
    }
    else
    {
      m_preDel_R = m_preDel_R_base + (m_preDel_R_diff * m_preDel_R_ramp);
      m_preDel_R_ramp += m_smooth_inc;
    }
    // lpf omega smoother
    if(m_lp_omega_ramp > 1.0f)
    {
      m_lp_omega = m_lp_omega_target;
    }
    else
    {
      m_lp_omega = m_lp_omega_base + (m_lp_omega_diff * m_lp_omega_ramp);
      m_lp_omega_ramp += m_smooth_inc;
    }
    m_lp_a0 = 1.0f / (m_lp_omega + 1.0f);
    m_lp_a1 = m_lp_omega - 1.0f;
    // hpf omega smoother
    if(m_hp_omega_ramp > 1.0f)
    {
      m_hp_omega = m_hp_omega_target;
    }
    else
    {
      m_hp_omega = m_hp_omega_base + (m_hp_omega_diff * m_hp_omega_ramp);
      m_hp_omega_ramp += m_smooth_inc;
    }
    m_hp_a0 = 1.0f / (m_hp_omega + 1.0f);
    m_hp_a1 = m_hp_omega - 1.0f;
    // lfos
    tmpVar = m_lfo_stateVar_1 + m_lfo_omega_1;
    tmpVar = tmpVar - std::round(tmpVar);
    m_lfo_stateVar_1 = tmpVar;
    tmpVar = (8.0f - std::abs(tmpVar) * 16.0f) * tmpVar;
    tmpVar += 1.0f;
    m_mod_1a = tmpVar * m_depth;
    m_mod_2a = (1.0f - tmpVar) * m_depth;
    tmpVar = m_lfo_stateVar_2 + m_lfo_omega_2;
    tmpVar = tmpVar - std::round(tmpVar);
    m_lfo_stateVar_2 = tmpVar;
    tmpVar = (8.0f - std::abs(tmpVar) * 16.0f) * tmpVar;
    tmpVar += 1.0f;
    m_mod_1b = tmpVar * m_depth;
    m_mod_2b = (1.0f - tmpVar) * m_depth;
  }
  // perform smoother clock tick
  m_slow_tick = (m_slow_tick + 1) & m_slow_thrsh;
  // left channel - input
  wetSample_L = _rawSample_L * _signals.get(C15::Signals::Mono_Signals::Reverb_Send)
      * _signals.get(C15::Signals::Mono_Signals::Reverb_Feedback);
  // left channel - asym 2
  m_buffer_L[m_buffer_indx] = wetSample_L;
  tmpVar = std::clamp(m_preDel_L, 0.0f, static_cast<float>(m_buffer_sz_m1));  /// this can be in a setter!
  ind_t0 = static_cast<int32_t>(std::round(tmpVar - 0.5f));
  tmpVar = tmpVar - static_cast<float>(ind_t0);
  ind_tm1 = ind_t0 + 1;
  ind_t0 = (m_buffer_indx - ind_t0) & m_buffer_sz_m1;
  ind_tm1 = (m_buffer_indx - ind_tm1) & m_buffer_sz_m1;
  wetSample_L = m_buffer_L[ind_t0] + tmpVar * (m_buffer_L[ind_tm1] - m_buffer_L[ind_t0]);
  wetSample_L += (m_stateVar_R9 * m_fb_amnt);
  // left channel - loop filter
  wetSample_L = (wetSample_L - (m_lp_stateVar_L * m_lp_a1)) * m_lp_a0;
  tmpVar = m_lp_stateVar_L;
  m_lp_stateVar_L = wetSample_L;
  wetSample_L = (wetSample_L + tmpVar) * m_lp_omega;
  wetSample_L = (wetSample_L - (m_hp_stateVar_L * m_hp_a1)) * m_hp_a0;
  tmpVar = m_hp_stateVar_L;
  m_hp_stateVar_L = wetSample_L;
  wetSample_L -= tmpVar;
  // left channel - delay (1) 4P
  tmpVar = m_stateVar_L1 * m_absorb;
  wetSample_L += tmpVar * REV_G_1;
  m_buffer_L1[m_buffer_indx] = wetSample_L;
  wetSample_L = (wetSample_L * -REV_G_1) + tmpVar;
  tmpVar = REV_DEL_L1 + m_mod_2a;
  tmpVar = std::clamp(tmpVar, 1.0f, static_cast<float>(m_buffer_sz_m2));
  ind_t0 = static_cast<int32_t>(std::round(tmpVar - 0.5f));
  tmpVar = tmpVar - static_cast<float>(ind_t0);
  ind_tm1 = std::max(ind_t0, 1) - 1;
  ind_tp1 = ind_t0 + 1;
  ind_tp2 = ind_t0 + 2;
  ind_tm1 = (m_buffer_indx - ind_tm1) & m_buffer_sz_m1;
  ind_t0 = (m_buffer_indx - ind_t0) & m_buffer_sz_m1;
  ind_tp1 = (m_buffer_indx - ind_tp1) & m_buffer_sz_m1;
  ind_tp2 = (m_buffer_indx - ind_tp2) & m_buffer_sz_m1;
  m_stateVar_L1 = NlToolbox::Math::interpolRT(tmpVar, m_buffer_L1[ind_tm1], m_buffer_L1[ind_t0], m_buffer_L1[ind_tp1],
                                              m_buffer_L1[ind_tp2]);
  // left channel - delay (2) 1P
  tmpVar = m_stateVar_L2 * m_absorb;
  wetSample_L += tmpVar * REV_G_2;
  m_buffer_L2[m_buffer_indx] = wetSample_L;
  wetSample_L = (wetSample_L * -REV_G_2) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_L2) & m_buffer_sz_m1;
  m_stateVar_L2 = m_buffer_L2[ind_t0];
  // left channel - delay (3) 1P
  tmpVar = m_stateVar_L3 * m_absorb;
  wetSample_L += tmpVar * REV_G_3;
  m_buffer_L3[m_buffer_indx] = wetSample_L;
  wetSample_L = (wetSample_L * -REV_G_3) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_L3) & m_buffer_sz_m1;
  m_stateVar_L3 = m_buffer_L3[ind_t0];
  // left channel - delay (4) 1P
  tmpVar = m_stateVar_L4 * m_absorb;
  wetSample_L += tmpVar * REV_G_4;
  m_buffer_L4[m_buffer_indx] = wetSample_L;
  wetSample_L2 = (wetSample_L * -REV_G_4) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_L4) & m_buffer_sz_m1;
  m_stateVar_L4 = m_buffer_L4[ind_t0];
  // left channel - delay (5) 1P
  tmpVar = m_stateVar_L5 * m_absorb;
  wetSample_L = wetSample_L2 + (tmpVar * REV_G_4);
  m_buffer_L5[m_buffer_indx] = wetSample_L;
  wetSample_L = (wetSample_L * -REV_G_4) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_L5) & m_buffer_sz_m1;
  m_stateVar_L5 = m_buffer_L5[ind_t0];
  // left channel - delay (6) 1P
  tmpVar = m_stateVar_L6 * m_absorb;
  wetSample_L += tmpVar * REV_G_4;
  m_buffer_L6[m_buffer_indx] = wetSample_L;
  wetSample_L = (wetSample_L * -REV_G_4) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_L6) & m_buffer_sz_m1;
  m_stateVar_L6 = m_buffer_L6[ind_t0];
  // left channel - delay (7) 1P
  tmpVar = m_stateVar_L7 * m_absorb;
  wetSample_L += tmpVar * REV_G_4;
  m_buffer_L7[m_buffer_indx] = wetSample_L;
  wetSample_L = (wetSample_L * -REV_G_4) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_L7) & m_buffer_sz_m1;
  m_stateVar_L7 = m_buffer_L7[ind_t0];
  // left channel - delay (8) 1P
  tmpVar = m_stateVar_L8 * m_absorb;
  wetSample_L += tmpVar * REV_G_4;
  m_buffer_L8[m_buffer_indx] = wetSample_L;
  wetSample_L = (wetSample_L * -REV_G_4) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_L8) & m_buffer_sz_m1;
  m_stateVar_L8 = m_buffer_L8[ind_t0];
  // right channel - input
  wetSample_R = _rawSample_R * _signals.get(C15::Signals::Mono_Signals::Reverb_Send)
      * _signals.get(C15::Signals::Mono_Signals::Reverb_Feedback);
  // right channel - asym 2
  m_buffer_R[m_buffer_indx] = wetSample_R;
  tmpVar = std::clamp(m_preDel_R, 0.0f, static_cast<float>(m_buffer_sz_m1));
  ind_t0 = static_cast<int32_t>(std::round(tmpVar - 0.5f));
  tmpVar = tmpVar - static_cast<float>(ind_t0);
  ind_tm1 = ind_t0 + 1;
  ind_t0 = (m_buffer_indx - ind_t0) & m_buffer_sz_m1;
  ind_tm1 = (m_buffer_indx - ind_tm1) & m_buffer_sz_m1;
  wetSample_R = m_buffer_R[ind_t0] + tmpVar * (m_buffer_R[ind_tm1] - m_buffer_R[ind_t0]);
  wetSample_R += (m_stateVar_L9 * m_fb_amnt);
  // right channel - loop filter
  wetSample_R = (wetSample_R - (m_lp_stateVar_R * m_lp_a1)) * m_lp_a0;
  tmpVar = m_lp_stateVar_R;
  m_lp_stateVar_R = wetSample_R;
  wetSample_R = (wetSample_R + tmpVar) * m_lp_omega;
  wetSample_R = (wetSample_R - (m_hp_stateVar_R * m_hp_a1)) * m_hp_a0;
  tmpVar = m_hp_stateVar_R;
  m_hp_stateVar_R = wetSample_R;
  wetSample_R -= tmpVar;
  // right channel - delay (1) 4P
  tmpVar = m_stateVar_R1 * m_absorb;
  wetSample_R += tmpVar * REV_G_1;
  m_buffer_R1[m_buffer_indx] = wetSample_R;
  wetSample_R = (wetSample_R * -REV_G_1) + tmpVar;
  tmpVar = REV_DEL_R1 + m_mod_2b;
  tmpVar = std::clamp(tmpVar, 1.0f, static_cast<float>(m_buffer_sz_m2));
  ind_t0 = static_cast<int32_t>(std::round(tmpVar - 0.5f));
  tmpVar = tmpVar - static_cast<float>(ind_t0);
  ind_tm1 = std::max(ind_t0, 1) - 1;
  ind_tp1 = ind_t0 + 1;
  ind_tp2 = ind_t0 + 2;
  ind_tm1 = (m_buffer_indx - ind_tm1) & m_buffer_sz_m1;
  ind_t0 = (m_buffer_indx - ind_t0) & m_buffer_sz_m1;
  ind_tp1 = (m_buffer_indx - ind_tp1) & m_buffer_sz_m1;
  ind_tp2 = (m_buffer_indx - ind_tp2) & m_buffer_sz_m1;
  m_stateVar_R1 = NlToolbox::Math::interpolRT(tmpVar, m_buffer_R1[ind_tm1], m_buffer_R1[ind_t0], m_buffer_R1[ind_tp1],
                                              m_buffer_R1[ind_tp2]);
  // right channel - delay (2) 1P
  tmpVar = m_stateVar_R2 * m_absorb;
  wetSample_R += tmpVar * REV_G_2;
  m_buffer_R2[m_buffer_indx] = wetSample_R;
  wetSample_R = (wetSample_R * -REV_G_2) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_R2) & m_buffer_sz_m1;
  m_stateVar_R2 = m_buffer_R2[ind_t0];
  // right channel - delay (3) 1P
  tmpVar = m_stateVar_R3 * m_absorb;
  wetSample_R += tmpVar * REV_G_3;
  m_buffer_R3[m_buffer_indx] = wetSample_R;
  wetSample_R = (wetSample_R * -REV_G_3) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_R3) & m_buffer_sz_m1;
  m_stateVar_R3 = m_buffer_R3[ind_t0];
  // right channel - delay (4) 1P
  tmpVar = m_stateVar_R4 * m_absorb;
  wetSample_R += tmpVar * REV_G_4;
  m_buffer_R4[m_buffer_indx] = wetSample_R;
  wetSample_R2 = (wetSample_R * -REV_G_4) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_R4) & m_buffer_sz_m1;
  m_stateVar_R4 = m_buffer_R4[ind_t0];
  // right channel - delay (5) 1P
  tmpVar = m_stateVar_R5 * m_absorb;
  wetSample_R = wetSample_R2 + (tmpVar * REV_G_4);
  m_buffer_R5[m_buffer_indx] = wetSample_R;
  wetSample_R = (wetSample_R * -REV_G_4) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_R5) & m_buffer_sz_m1;
  m_stateVar_R5 = m_buffer_R5[ind_t0];
  // right channel - delay (6) 1P
  tmpVar = m_stateVar_R6 * m_absorb;
  wetSample_R += tmpVar * REV_G_4;
  m_buffer_R6[m_buffer_indx] = wetSample_R;
  wetSample_R = (wetSample_R * -REV_G_4) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_R6) & m_buffer_sz_m1;
  m_stateVar_R6 = m_buffer_R6[ind_t0];
  // right channel - delay (7) 1P
  tmpVar = m_stateVar_R7 * m_absorb;
  wetSample_R += tmpVar * REV_G_4;
  m_buffer_R7[m_buffer_indx] = wetSample_R;
  wetSample_R = (wetSample_R * -REV_G_4) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_R7) & m_buffer_sz_m1;
  m_stateVar_R7 = m_buffer_R7[ind_t0];
  // right channel - delay (8) 1P
  tmpVar = m_stateVar_R8 * m_absorb;
  wetSample_R += tmpVar * REV_G_4;
  m_buffer_R8[m_buffer_indx] = wetSample_R;
  wetSample_R = (wetSample_R * -REV_G_4) + tmpVar;
  ind_t0 = (m_buffer_indx - REV_DEL_R8) & m_buffer_sz_m1;
  m_stateVar_R8 = m_buffer_R8[ind_t0];
  // feedback delays
  // left channel - delay (9) 4P
  m_buffer_L9[m_buffer_indx] = wetSample_L;
  tmpVar = REV_DEL_L9 + m_mod_1a;
  tmpVar = std::clamp(tmpVar, 0.0f, static_cast<float>(m_buffer_sz_m2));
  ind_t0 = static_cast<int32_t>(std::round(tmpVar - 0.5f));
  tmpVar = tmpVar - static_cast<float>(ind_t0);
  ind_tm1 = std::max(ind_t0, 1) - 1;
  ind_tp1 = ind_t0 + 1;
  ind_tp2 = ind_t0 + 2;
  ind_tm1 = (m_buffer_indx - ind_tm1) & m_buffer_sz_m1;
  ind_t0 = (m_buffer_indx - ind_t0) & m_buffer_sz_m1;
  ind_tp1 = (m_buffer_indx - ind_tp1) & m_buffer_sz_m1;
  ind_tp2 = (m_buffer_indx - ind_tp2) & m_buffer_sz_m1;
  m_stateVar_L9 = NlToolbox::Math::interpolRT(tmpVar, m_buffer_L9[ind_tm1], m_buffer_L9[ind_t0], m_buffer_L9[ind_tp1],
                                              m_buffer_L9[ind_tp2]);
  // right channel - delay (9) 4P
  m_buffer_R9[m_buffer_indx] = wetSample_R;
  tmpVar = REV_DEL_R9 + m_mod_1b;
  tmpVar = std::clamp(tmpVar, 0.0f, static_cast<float>(m_buffer_sz_m2));
  ind_t0 = static_cast<int32_t>(std::round(tmpVar - 0.5f));
  tmpVar = tmpVar - static_cast<float>(ind_t0);
  ind_tm1 = std::max(ind_t0, 1) - 1;
  ind_tp1 = ind_t0 + 1;
  ind_tp2 = ind_t0 + 2;
  ind_tm1 = (m_buffer_indx - ind_tm1) & m_buffer_sz_m1;
  ind_t0 = (m_buffer_indx - ind_t0) & m_buffer_sz_m1;
  ind_tp1 = (m_buffer_indx - ind_tp1) & m_buffer_sz_m1;
  ind_tp2 = (m_buffer_indx - ind_tp2) & m_buffer_sz_m1;
  m_stateVar_R9 = NlToolbox::Math::interpolRT(tmpVar, m_buffer_R9[ind_tm1], m_buffer_R9[ind_t0], m_buffer_R9[ind_tp1],
                                              m_buffer_R9[ind_tp2]);
  // increase delay buffer index
  m_buffer_indx = (m_buffer_indx + 1) & m_buffer_sz_m1;
  // output values
  wetSample_L = (wetSample_L * m_bal_full) + (wetSample_L2 * m_bal_half);
  wetSample_R = (wetSample_R * m_bal_full) + (wetSample_R2 * m_bal_half);
  m_out_L = (_rawSample_L * _signals.get(C15::Signals::Mono_Signals::Reverb_Dry))
      + (wetSample_L * _signals.get(C15::Signals::Mono_Signals::Reverb_Wet));
  m_out_R = (_rawSample_R * _signals.get(C15::Signals::Mono_Signals::Reverb_Dry))
      + (wetSample_R * _signals.get(C15::Signals::Mono_Signals::Reverb_Wet));
  m_out_dry = _rawSample_L + _rawSample_R;
  m_out_wet = wetSample_L + wetSample_R;
}

void Engine::MonoReverb::resetDSP()
{
  // output values
  m_out_L = m_out_R = m_out_dry = m_out_wet = 0.0f;
  // loop filter
  m_lp_stateVar_L = m_lp_stateVar_R = m_hp_stateVar_L = m_hp_stateVar_R = 0.0f;
  // delay buffer - flush left buffers
  std::fill(m_buffer_L.begin(), m_buffer_L.end(), 0.0f);
  std::fill(m_buffer_L1.begin(), m_buffer_L1.end(), 0.0f);
  std::fill(m_buffer_L2.begin(), m_buffer_L2.end(), 0.0f);
  std::fill(m_buffer_L3.begin(), m_buffer_L3.end(), 0.0f);
  std::fill(m_buffer_L4.begin(), m_buffer_L4.end(), 0.0f);
  std::fill(m_buffer_L5.begin(), m_buffer_L5.end(), 0.0f);
  std::fill(m_buffer_L6.begin(), m_buffer_L6.end(), 0.0f);
  std::fill(m_buffer_L7.begin(), m_buffer_L7.end(), 0.0f);
  std::fill(m_buffer_L8.begin(), m_buffer_L8.end(), 0.0f);
  std::fill(m_buffer_L9.begin(), m_buffer_L9.end(), 0.0f);
  // delay buffer - flush right buffers
  std::fill(m_buffer_R.begin(), m_buffer_R.end(), 0.0f);
  std::fill(m_buffer_R1.begin(), m_buffer_R1.end(), 0.0f);
  std::fill(m_buffer_R2.begin(), m_buffer_R2.end(), 0.0f);
  std::fill(m_buffer_R3.begin(), m_buffer_R3.end(), 0.0f);
  std::fill(m_buffer_R4.begin(), m_buffer_R4.end(), 0.0f);
  std::fill(m_buffer_R5.begin(), m_buffer_R5.end(), 0.0f);
  std::fill(m_buffer_R6.begin(), m_buffer_R6.end(), 0.0f);
  std::fill(m_buffer_R7.begin(), m_buffer_R7.end(), 0.0f);
  std::fill(m_buffer_R8.begin(), m_buffer_R8.end(), 0.0f);
  std::fill(m_buffer_R9.begin(), m_buffer_R9.end(), 0.0f);
  // delay buffer - clear state vars
  m_stateVar_L1 = m_stateVar_L2 = m_stateVar_L3 = m_stateVar_L4 = m_stateVar_L5 = m_stateVar_L6 = m_stateVar_L7
      = m_stateVar_L8 = m_stateVar_L9 = m_stateVar_R1 = m_stateVar_R2 = m_stateVar_R3 = m_stateVar_R4 = m_stateVar_R5
      = m_stateVar_R6 = m_stateVar_R7 = m_stateVar_R8 = m_stateVar_R9 = 0.0f;
}
