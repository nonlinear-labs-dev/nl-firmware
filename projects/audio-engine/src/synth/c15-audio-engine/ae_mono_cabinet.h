#pragma once

/******************************************************************************/
/** @file       ae_mono_cabinet.h
    @date
    @version    1.7-3
    @author     A. Schmied, M. Seeber
    @brief      ...
    @todo
*******************************************************************************/

#include "nltoolbox.h"
#include "ae_info.h"

namespace Engine
{
  class MonoCabinet
  {
   public:
    float m_out_L, m_out_R;
    MonoCabinet();
    void init(const float _samplerate);
    void set(MonoSignals &_signals);
    void apply(MonoSignals &_signals, const float _rawSample_L, const float _rawSample_R);
    void resetDSP();

   private:
    // prewarp
    float m_warpConst_2PI;
    float m_freqClip_max, m_freqClip_min;
    // biquad hp
    float m_hp_b0, m_hp_b1;
    float m_hp_a1, m_hp_a2;
    float m_hp_stateVar_L1, m_hp_stateVar_R1;
    float m_hp_stateVar_L2, m_hp_stateVar_R2;
    float m_hp_stateVar_L3, m_hp_stateVar_R3;
    float m_hp_stateVar_L4, m_hp_stateVar_R4;
    // biquad lp
    float m_lp1_b0, m_lp1_b1;
    float m_lp1_a1, m_lp1_a2;
    float m_lp1_stateVar_L1, m_lp1_stateVar_R1;
    float m_lp1_stateVar_L2, m_lp1_stateVar_R2;
    float m_lp1_stateVar_L3, m_lp1_stateVar_R3;
    float m_lp1_stateVar_L4, m_lp1_stateVar_R4;
    float m_lp2_b0, m_lp2_b1;
    float m_lp2_a1, m_lp2_a2;
    float m_lp2_stateVar_L1, m_lp2_stateVar_R1;
    float m_lp2_stateVar_L2, m_lp2_stateVar_R2;
    float m_lp2_stateVar_L3, m_lp2_stateVar_R3;
    float m_lp2_stateVar_L4, m_lp2_stateVar_R4;
    // tilt low shelves
    float m_tiltOmegaSin, m_tiltOmegaCos;
    float m_ls1_b0, m_ls1_b1, m_ls1_b2;
    float m_ls1_a1, m_ls1_a2;
    float m_ls1_stateVar_L1, m_ls1_stateVar_R1;
    float m_ls1_StateVar_L2, m_ls1_stateVar_R2;
    float m_ls1_StateVar_L3, m_ls1_stateVar_R3;
    float m_ls1_StateVar_L4, m_ls1_stateVar_R4;
    float m_ls2_b0, m_ls2_b1, m_ls2_b2;
    float m_ls2_a1, m_ls2_a2;
    float m_ls2_stateVar_L1, m_ls2_stateVar_R1;
    float m_ls2_StateVar_L2, m_ls2_stateVar_R2;
    float m_ls2_StateVar_L3, m_ls2_stateVar_R3;
    float m_ls2_StateVar_L4, m_ls2_stateVar_R4;
    // 30 Hz hp
    float m_hp30_b0;
    float m_hp30_stateVar_L, m_hp30_stateVar_R;
  };
}  // namespace Engine
