#pragma once

/******************************************************************************/
/** @file       ae_mono_flanger.h
    @date
    @version    1.7-0
    @author     M. Seeber
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
    std::vector<float> m_buffer_L, m_buffer_R;
    float m_warpConst_PI, m_warpConst_2PI, m_freqClip_min, m_freqClip_max, m_lp_b0, m_lp_b1, m_lp_a1, m_lp_stateVar_L1,
        m_lp_stateVar_L2, m_lp_stateVar_R1, m_lp_stateVar_R2, m_hp_b0, m_hp_b1, m_hp_a1, m_hp_stateVar_L1,
        m_hp_stateVar_L2, m_hp_stateVar_R1, m_hp_stateVar_R2, m_lp2hz_b0, m_lp2hz_stateVar_TL, m_lp2hz_stateVar_TR,
        m_lp2hz_stateVar_D, m_ap_b0_L, m_ap_b1_L, m_ap_b0_R, m_ap_b1_R, m_ap_StateVar_L1, m_ap_StateVar_R1,
        m_ap_StateVar_L2, m_ap_StateVar_R2, m_ap_StateVar_L3, m_ap_StateVar_R3, m_ap_StateVar_L4, m_ap_StateVar_R4,
        m_ap_StateVar_L5, m_ap_StateVar_R5, m_ap_StateVar_L6, m_ap_StateVar_R6, m_ap_StateVar_L7, m_ap_StateVar_R7,
        m_ap_StateVar_L8, m_ap_StateVar_R8, m_stateVar_L, m_stateVar_R;
    int32_t m_buffer_indx, m_buffer_sz_m1;
  };
}  // namespace Engine
