#pragma once

/******************************************************************************/
/** @file       ae_mono_gapfilter.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      ...
    @todo
*******************************************************************************/

#include "nltoolbox.h"
#include "ae_info.h"

namespace Engine
{
  class MonoGapFilter
  {
   public:
    float m_out_L, m_out_R;
    MonoGapFilter();
    void init(const float _samplerate);
    void apply(MonoSignals &_signals, const float _rawSample_L, const float _rawSample_R);
    void set(MonoSignals &_signals);
    void resetDSP();

   private:
    float m_warpConst_2PI, m_freqClip_max, m_freqClip_min, m_freqScale_max, m_freqScale_min, m_hp_l1_b0, m_hp_l1_b1,
        m_hp_l2_b0, m_hp_l2_b1, m_hp_l1_a1, m_hp_l1_a2, m_hp_l2_a1, m_hp_l2_a2, m_hp_l1_stateVar_1, m_hp_l2_stateVar_1,
        m_hp_l1_stateVar_2, m_hp_l2_stateVar_2, m_hp_l1_stateVar_3, m_hp_l2_stateVar_3, m_hp_l1_stateVar_4,
        m_hp_l2_stateVar_4, m_hp_r1_b0, m_hp_r1_b1, m_hp_r2_b0, m_hp_r2_b1, m_hp_r1_a1, m_hp_r1_a2, m_hp_r2_a1,
        m_hp_r2_a2, m_hp_r1_stateVar_1, m_hp_r2_stateVar_1, m_hp_r1_stateVar_2, m_hp_r2_stateVar_2, m_hp_r1_stateVar_3,
        m_hp_r2_stateVar_3, m_hp_r1_stateVar_4, m_hp_r2_stateVar_4, m_lp_l1_b0, m_lp_l1_b1, m_lp_l2_b0, m_lp_l2_b1,
        m_lp_l1_a1, m_lp_l1_a2, m_lp_l2_a1, m_lp_l2_a2, m_lp_l1_stateVar_1, m_lp_l2_stateVar_1, m_lp_l1_stateVar_2,
        m_lp_l2_stateVar_2, m_lp_l1_stateVar_3, m_lp_l2_stateVar_3, m_lp_l1_stateVar_4, m_lp_l2_stateVar_4, m_lp_r1_b0,
        m_lp_r1_b1, m_lp_r2_b0, m_lp_r2_b1, m_lp_r1_a1, m_lp_r1_a2, m_lp_r2_a1, m_lp_r2_a2, m_lp_r1_stateVar_1,
        m_lp_r2_stateVar_1, m_lp_r1_stateVar_2, m_lp_r2_stateVar_2, m_lp_r1_stateVar_3, m_lp_r2_stateVar_3,
        m_lp_r1_stateVar_4, m_lp_r2_stateVar_4;
  };
}  // namespace Engine
