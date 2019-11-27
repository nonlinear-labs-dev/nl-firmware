#pragma once

/******************************************************************************/
/** @file       ae_mono_reverb.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      ...
    @todo
*******************************************************************************/

#include <vector>
#include "nltoolbox.h"
#include "ae_info.h"

#define REVERB_BUFFER_SIZE 16384

#define REV_G_1 0.617748f
#define REV_G_2 0.630809f
#define REV_G_3 0.64093f
#define REV_G_4 0.653011f

// Primes - 1 for 48 kHz
#define REV_DEL_L1 280   // 563
#define REV_DEL_L2 1122  // 2243
#define REV_DEL_L3 862   // 1723
#define REV_DEL_L4 466   // 937
#define REV_DEL_L5 718   // 1439
#define REV_DEL_L6 1030  // 2063
#define REV_DEL_L7 886   // 1777
#define REV_DEL_L8 1216  // 2437
#define REV_DEL_R1 378   // 757
#define REV_DEL_R2 1102  // 2003
#define REV_DEL_R3 928   // 1861
#define REV_DEL_R4 490   // 577
#define REV_DEL_R5 682   // 1361
#define REV_DEL_R6 1018  // 2039
#define REV_DEL_R7 858   // 1721
#define REV_DEL_R8 1366  // 1733

#define REV_DEL_L9 2916  // 5827
#define REV_DEL_R9 2676  // 5351

namespace Engine
{
  class MonoReverb
  {
   public:
    float m_out_L, m_out_R, m_out_dry, m_out_wet;
    MonoReverb();
    void init(const float _samplerate, const uint32_t _upsampleFactor);
    void apply(MonoSignals &_signals, const float _rawSample_L, const float _rawSample_R);
    void set(MonoSignals &_signals);
    void resetDSP();

   private:
    // delay buffer
    std::vector<float> m_buffer_L, m_buffer_L1, m_buffer_L2, m_buffer_L3, m_buffer_L4, m_buffer_L5, m_buffer_L6,
        m_buffer_L7, m_buffer_L8, m_buffer_L9, m_buffer_R, m_buffer_R1, m_buffer_R2, m_buffer_R3, m_buffer_R4,
        m_buffer_R5, m_buffer_R6, m_buffer_R7, m_buffer_R8, m_buffer_R9;
    float
        // reverb modulation
        m_mod_1a,
        m_mod_2a, m_mod_1b, m_mod_2b, m_lfo_omega_1, m_lfo_omega_2, m_lfo_stateVar_1, m_lfo_stateVar_2, m_depth,
        // loop filter
        m_warpConst_PI, m_omegaClip_max, m_lp_a0, m_lp_a1, m_lp_omega, m_hp_a0, m_hp_a1, m_hp_omega, m_lp_stateVar_L,
        m_lp_stateVar_R, m_hp_stateVar_L, m_hp_stateVar_R,
        // delay buffer
        m_fb_amnt, m_absorb, m_bal_half, m_bal_full, m_preDel_L, m_preDel_R, m_stateVar_L1, m_stateVar_R1,
        m_stateVar_L2, m_stateVar_R2, m_stateVar_L3, m_stateVar_R3, m_stateVar_L4, m_stateVar_R4, m_stateVar_L5,
        m_stateVar_R5, m_stateVar_L6, m_stateVar_R6, m_stateVar_L7, m_stateVar_R7, m_stateVar_L8, m_stateVar_R8,
        m_stateVar_L9, m_stateVar_R9,
        // smoothing
        m_smooth_inc, m_depth_inc, m_depth_ramp, m_depth_target, m_depth_base, m_depth_diff, m_size, m_size_ramp,
        m_size_target, m_size_base, m_size_diff, m_bal, m_bal_ramp, m_bal_target, m_bal_base, m_bal_diff,
        m_preDel_L_ramp, m_preDel_L_target, m_preDel_L_base, m_preDel_L_diff, m_preDel_R_ramp, m_preDel_R_target,
        m_preDel_R_base, m_preDel_R_diff, m_lp_omega_ramp, m_lp_omega_target, m_lp_omega_base, m_lp_omega_diff,
        m_hp_omega_ramp, m_hp_omega_target, m_hp_omega_base, m_hp_omega_diff;
    // clock division and delay buffer indices
    uint32_t m_slow_tick, m_slow_thrsh, m_buffer_indx, m_buffer_sz_m1, m_buffer_sz_m2;
  };
}  // namespace Engine
