#pragma once

/******************************************************************************/
/** @file       ae_poly_fb_mix.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      feedback mixer
    @todo
*******************************************************************************/

#include "nltoolbox.h"
#include "ae_info.h"

// todo: FX I/II and Osc Cross handling

namespace Engine
{
  class PolyFeedbackMixer
  {
   public:
    PolyValue m_out;
    PolyFeedbackMixer();
    void init(const float _samplerate);
    void set(PolySignals &_signals, const uint32_t _voiceId);
    void apply(PolySignals &_signals, const PolyValue &_oscA, const PolyValue &_oscB, const PolyValue &_cmb0,
               const PolyValue &_cmb1, const PolyValue &_svf0, const PolyValue &_svf1, const float _fx0_dry,
               const float _fx0_wet, const float _fx1_dry, const float _fx1_wet);
    void resetDSP();

   private:
    PolyValue m_hp30hz_b0, m_hp30hz_stateVar, m_hp_b0, m_hp_b1, m_hp_a1, m_hp_stateVar_1, m_hp_stateVar_2;
    float m_warpConst_PI, m_freqClip_min, m_freqClip_max;
  };
}  // namespace Engine
