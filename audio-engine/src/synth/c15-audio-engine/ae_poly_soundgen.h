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
    PolyValue m_out_A, m_out_B;
    PolySoundGenerator();
    void init(const uint32_t _voices, const float _samplerate);
    void set(PolySignals &_signals, const uint32_t _voiceId);
    void generate(PolySignals &_signals, const float _feedbackSample);
    void resetPhase(uint32_t _voiceID);
    void resetDSP();

   private:
    PolyValue m_sample_interval;
    PolyValue m_feedback_phase;
    PolyValue m_mute_state_A{ 1 };
    PolyValue m_mute_state_B{ 1 };
    PolyValue m_oscA_selfmix;
    PolyValue m_oscA_crossmix;
    PolyValue m_oscA_phase;
    PolyValue m_oscA_phase_stateVar;
    PolyValue m_oscA_phaseInc;
    PolyInt m_OscA_randVal_int;
    PolyValue m_OscA_randVal_float;
    PolyValue m_oscB_selfmix;
    PolyValue m_oscB_crossmix;
    PolyValue m_oscB_phase;
    PolyValue m_oscB_phase_stateVar;
    PolyValue m_oscB_phaseInc;
    PolyInt m_OscB_randVal_int;
    PolyValue m_OscB_randVal_float;
    float m_warpConst_PI;
    PolyValue m_chiA_stateVar, m_chiB_stateVar;
    PolyValue m_chiA_omega, m_chiA_a0, m_chiA_a1;
    PolyValue m_chiB_omega, m_chiB_a0, m_chiB_a1;
    uint32_t m_voices;
  };
}  // namespace Engine
