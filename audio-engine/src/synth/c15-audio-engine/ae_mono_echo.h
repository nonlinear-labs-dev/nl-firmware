#pragma once

/******************************************************************************/
/** @file       ae_mono_echo.h
    @date
    @version    1.7-3
    @author     A. Schmied, M. Seeber
    @brief      ...
    @todo
*******************************************************************************/

#include <vector>
#include "nltoolbox.h"
#include "ae_info.h"

#define ECHO_BUFFER_SIZE 131072

namespace Engine
{
  class MonoEcho
  {
   public:
    float m_out_L, m_out_R;
    MonoEcho();
    void init(const float _samplerate, const uint32_t _upsampleFactor);
    void set(MonoSignals &_signals);
    void apply(MonoSignals &_signals, const float _rawSample_L, const float _rawSample_R);
    void resetDSP();

   private:
    // prewarp
    float m_warpConst_PI;
    float m_freqClip_min, m_freqClip_max;
    // 1p hp
    float m_hp_b0, m_hp_b1, m_hp_a1;
    float m_hp_stateVar_L1, m_hp_stateVar_L2;
    float m_hp_stateVar_R1, m_hp_stateVar_R2;
    // 1p lp
    float m_lp_b0, m_lp_b1, m_lp_a1;
    float m_lp_stateVar_L1, m_lp_stateVar_L2;
    float m_lp_stateVar_R1, m_lp_stateVar_R2;
    // 2 Hz lp
    float m_lp2hz_b0;
    float m_lp2hz_stateVar_L, m_lp2hz_stateVar_R;
    // delay buffer
    float m_stateVar_L, m_stateVar_R;
    int32_t m_buffer_indx;
    int32_t m_buffer_sz_m1;
    std::vector<float> m_buffer_L;
    std::vector<float> m_buffer_R;
  };
}  // namespace Engine
