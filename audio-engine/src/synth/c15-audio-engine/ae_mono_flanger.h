#pragma once

/******************************************************************************/
/** @file       ae_mono_flanger.h
    @date
    @version    1.7-3
    @author     A. Schmied, M. Seeber
    @brief      ...
    @todo
*******************************************************************************/

#include <vector>
#include "nltoolbox.h"
#include "ae_info.h"

#define FLANGER_BUFFER_SIZE 8192

namespace Engine
{
  class MonoFlanger
  {
   public:
    float m_out_L, m_out_R;
    MonoFlanger();
    void init(const float _samplerate, const uint32_t _upsampleFactor);
    void set_slow(MonoSignals &_signals);
    void set_fast(MonoSignals &_signals);
    void apply(MonoSignals &_signals, const float _rawSample_L, const float _rawSample_R);
    void resetDSP();

   private:
    // prewarp
    float m_warpConst_PI, m_warpConst_2PI;
    float m_freqClip_min;
    float m_freqClip_max;
    // 1p lp
    float m_lp_b0, m_lp_b1, m_lp_a1;
    float m_lp_stateVar_L1, m_lp_stateVar_L2;
    float m_lp_stateVar_R1, m_lp_stateVar_R2;
    // 1p hp
    float m_hp_b0, m_hp_b1, m_hp_a1;
    float m_hp_stateVar_L1, m_hp_stateVar_L2;
    float m_hp_stateVar_R1, m_hp_stateVar_R2;
    // 2 Hz lp
    float m_lp2hz_b0;
    float m_lp2hz_stateVar_TL, m_lp2hz_stateVar_TR, m_lp2hz_stateVar_D;
    // 4p ap
    float m_ap_b0_L, m_ap_b1_L;
    float m_ap_b0_R, m_ap_b1_R;
    float m_ap_StateVar_L1, m_ap_StateVar_R1;
    float m_ap_StateVar_L2, m_ap_StateVar_R2;
    float m_ap_StateVar_L3, m_ap_StateVar_R3;
    float m_ap_StateVar_L4, m_ap_StateVar_R4;
    float m_ap_StateVar_L5, m_ap_StateVar_R5;
    float m_ap_StateVar_L6, m_ap_StateVar_R6;
    float m_ap_StateVar_L7, m_ap_StateVar_R7;
    float m_ap_StateVar_L8, m_ap_StateVar_R8;
    // delay buffer
    float m_stateVar_L, m_stateVar_R;
    int32_t m_buffer_indx;
    int32_t m_buffer_sz_m1;
    std::vector<float> m_buffer_L;
    std::vector<float> m_buffer_R;
  };
}  // namespace Engine
