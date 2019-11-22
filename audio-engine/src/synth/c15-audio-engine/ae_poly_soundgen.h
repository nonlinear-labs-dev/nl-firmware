#pragma once

/******************************************************************************/
/** @file       ae_poly_soundgen.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new soundgenerator
    @todo
*******************************************************************************/

#include <cmath>
#include "nltoolbox.h"
#include "pe_defines_config.h"
#include "ae_info.h"

namespace Engine
{
  class PolySoundGenerator
  {
   public:
    PolyValue m_out_A, m_out_B, m_feedback_phase;
    PolySoundGenerator();
    void init(const uint32_t _voices, const float _samplerate);
    void set(PolySignals &_signals, const uint32_t _voiceId);
    void generate(PolySignals &_signals, const PolyValue _feedbackSample);
    void resetPhase(uint32_t _voiceID);
    void resetDSP();

   private:
    PolyValue m_sample_interval, m_mute_state_A{ 1 }, m_mute_state_B{ 1 }, m_oscA_selfmix, m_oscA_crossmix,
        m_oscA_phase, m_oscA_phase_stateVar, m_oscA_phaseInc, m_OscA_randVal_float, m_oscB_selfmix, m_oscB_crossmix,
        m_oscB_phase, m_oscB_phase_stateVar, m_oscB_phaseInc, m_OscB_randVal_float, m_chiA_stateVar, m_chiB_stateVar,
        m_chiA_omega, m_chiA_a0, m_chiA_a1, m_chiB_omega, m_chiB_a0, m_chiB_a1;
    PolyInt m_OscA_randVal_int, m_OscB_randVal_int;
    float m_warpConst_PI;
    uint32_t m_voices;
  };
}  // namespace Engine
