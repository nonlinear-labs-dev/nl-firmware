/******************************************************************************/
/** @file           ae_soundgenerator.cpp
    @date           2018-03-22
    @version        1.0
    @author         Anton Schmied
    @brief          Soundgenerator which contains both Oscillators and Shapers
                    receiving the rendered parameter signals from the
                    param engine
    @todo
*******************************************************************************/

#include "ae_soundgenerator.h"
#include "SignalStorage.h"

/******************************************************************************/
/** @brief
*******************************************************************************/

ae_soundgenerator::ae_soundgenerator()
{
}

/******************************************************************************/
/** @brief
*******************************************************************************/

void ae_soundgenerator::init(float _samplerate)
{
  m_out_A = 0.f;
  m_out_B = 0.f;

  m_sample_interval = 1.f / _samplerate;
  m_warpConst_PI = 3.14159f / _samplerate;

  m_feedback_phase = 0.f;

  m_chiA_stateVar = 0.f;
  m_chiB_stateVar = 0.f;

  m_chiA_omega = 0.f;
  m_chiA_a0 = 0.f;
  m_chiA_a1 = 0.f;
  m_chiB_omega = 0.f;
  m_chiB_a0 = 0.f;
  m_chiB_a1 = 0.f;

  for(int vn = 0; vn < dsp_number_of_voices; vn++)
  {
    m_OscA_randVal_int = vn + 1;
    m_OscB_randVal_int = vn + 1 + 111;
  }

  m_OscA_mute = 0u;
  m_OscB_mute = 0u;
}

/******************************************************************************/
/** @brief
*******************************************************************************/
void ae_soundgenerator::set(SignalStorage &signals, uint32_t voice)
{
  //*************************** Chirp Filter A *****************************//
  m_chiA_omega[voice] = signals.get<Signals::OSC_A_CHI>() * m_warpConst_PI;
  m_chiA_omega[voice] = NlToolbox::Math::tan(m_chiA_omega[voice]);

  m_chiA_a0[voice] = 1.f / (m_chiA_omega[voice] + 1.f);
  m_chiA_a1[voice] = m_chiA_omega[voice] - 1.f;

  //*************************** Chirp Filter B *****************************//
  m_chiB_omega[voice] = signals.get<Signals::OSC_B_CHI>() * m_warpConst_PI;
  m_chiB_omega[voice] = NlToolbox::Math::tan(m_chiB_omega[voice]);

  m_chiB_a0[voice] = 1.f / (m_chiB_omega[voice] + 1.f);
  m_chiB_a1[voice] = m_chiB_omega[voice] - 1.f;
}

/******************************************************************************/
/** @brief
*******************************************************************************/

void ae_soundgenerator::resetPhase(float _phase, uint32_t _voiceID)
{
  m_oscA_phase[_voiceID] = _phase;
  m_oscB_phase[_voiceID] = _phase;
  /* */
  m_oscA_selfmix[_voiceID] = 0.f;
  m_oscA_crossmix[_voiceID] = 0.f;
  m_chiA_stateVar[_voiceID] = 0.f;
  /* */
  m_oscB_selfmix[_voiceID] = 0.f;
  m_oscB_crossmix[_voiceID] = 0.f;
  m_chiB_stateVar[_voiceID] = 0.f;
  /* */
  m_feedback_phase[_voiceID] = 0.f;
}

/******************************************************************************/
/** @brief
*******************************************************************************/

void ae_soundgenerator::generate(const FloatVector &_feedbackSample, SignalStorage &signals)
{
  float tmpVar;

  //**************************** Modulation A ******************************//
  float oscSampleA = m_oscA_selfmix * signals.get<Signals::OSC_A_PMSEA>();
  oscSampleA = oscSampleA + m_oscB_crossmix * signals.get<Signals::OSC_A_PMBEB>();
  oscSampleA = oscSampleA + m_feedback_phase * signals.get<Signals::OSC_A_PMFEC>();

  //**************************** Oscillator A ******************************//
  oscSampleA -= (m_chiA_a1 * m_chiA_stateVar);  // Chirp IIR
  oscSampleA *= m_chiA_a0;

  tmpVar = oscSampleA;

  oscSampleA = (oscSampleA + m_chiA_stateVar) * m_chiA_omega;  // Chirp FIR
  m_chiA_stateVar = tmpVar + NlToolbox::Constants::DNC_const;

  oscSampleA += m_oscA_phase;

  oscSampleA += signals.get<Signals::OSC_A_PHS>() + signals.get<Signals::UN_PHS>();  // NEW Phase Offset
  oscSampleA += (-0.25f);                                                            // Wrap
  oscSampleA -= NlToolbox::Conversion::float2int(oscSampleA);

  if(std::abs(m_oscA_phase_stateVar - oscSampleA) > 0.5f)  // Check edge
  {
    m_OscA_randVal_int = m_OscA_randVal_int * 1103515245 + 12345;
    m_OscA_randVal_float = static_cast<float>(m_OscA_randVal_int) * 4.5657e-10f;
  }

  float osc_freq = signals.get<Signals::OSC_A_FRQ>();
  m_oscA_phaseInc
      = ((m_OscA_randVal_float * signals.get<Signals::OSC_A_FLUEC>() * osc_freq) + osc_freq) * m_sample_interval;

  m_oscA_phase_stateVar = oscSampleA;

  m_oscA_phase += m_oscA_phaseInc;
  m_oscA_phase -= NlToolbox::Conversion::float2int(m_oscA_phase);

  oscSampleA = m_mute_state[m_OscA_mute] * NlToolbox::Math::sinP3_noWrap(oscSampleA);

  //**************************** Modulation B ******************************//
  float oscSampleB = m_oscB_selfmix * signals.get<Signals::OSC_B_PMSEB>();
  oscSampleB = oscSampleB + m_oscA_crossmix * signals.get<Signals::OSC_B_PMAEA>();
  oscSampleB = oscSampleB + m_feedback_phase * signals.get<Signals::OSC_B_PMFEC>();

  //**************************** Oscillator B ******************************//
  oscSampleB -= (m_chiB_a1 * m_chiB_stateVar);  // Chirp IIR
  oscSampleB *= m_chiB_a0;

  tmpVar = oscSampleB;

  oscSampleB = (oscSampleB + m_chiB_stateVar) * m_chiB_omega;  // Chirp FIR
  m_chiB_stateVar = tmpVar + NlToolbox::Constants::DNC_const;

  oscSampleB += m_oscB_phase;

  oscSampleB += signals.get<Signals::OSC_B_PHS>() + signals.get<Signals::UN_PHS>();  // NEW Phase Offset
  oscSampleB += (-0.25f);                                                            // Warp
  oscSampleB -= NlToolbox::Conversion::float2int(oscSampleB);

  if(std::abs(m_oscB_phase_stateVar - oscSampleB) > 0.5f)  // Check edge
  {
    m_OscB_randVal_int = m_OscB_randVal_int * 1103515245 + 12345;
    m_OscB_randVal_float = static_cast<float>(m_OscB_randVal_int) * 4.5657e-10f;
  }

  osc_freq = signals.get<Signals::OSC_B_FRQ>();
  m_oscB_phaseInc
      = ((m_OscB_randVal_float * signals.get<Signals::OSC_B_FLUEC>() * osc_freq) + osc_freq) * m_sample_interval;

  m_oscB_phase_stateVar = oscSampleB;

  m_oscB_phase += m_oscB_phaseInc;
  m_oscB_phase -= NlToolbox::Conversion::float2int(m_oscB_phase);

  oscSampleB = m_mute_state[m_OscB_mute] * NlToolbox::Math::sinP3_noWrap(oscSampleB);

  //******************************* Shaper A *******************************//
  float shaperSampleA = oscSampleA * signals.get<Signals::SHP_A_DRVEA>();
  tmpVar = shaperSampleA;

  shaperSampleA = NlToolbox::Math::sinP3_wrap(shaperSampleA);
  shaperSampleA = NlToolbox::Others::threeRanges(shaperSampleA, tmpVar, signals.get<Signals::SHP_A_FLD>());

  tmpVar = shaperSampleA * shaperSampleA + (-0.5f);

  shaperSampleA = NlToolbox::Others::parAsym(shaperSampleA, tmpVar, signals.get<Signals::SHP_A_ASM>());

  //******************************* Shaper B *******************************//
  float shaperSampleB = oscSampleB * signals.get<Signals::SHP_B_DRVEB>();
  tmpVar = shaperSampleB;

  shaperSampleB = NlToolbox::Math::sinP3_wrap(shaperSampleB);
  shaperSampleB = NlToolbox::Others::threeRanges(shaperSampleB, tmpVar, signals.get<Signals::SHP_B_FLD>());

  tmpVar = shaperSampleB * shaperSampleB + (-0.5f);

  shaperSampleB = NlToolbox::Others::parAsym(shaperSampleB, tmpVar, signals.get<Signals::SHP_B_ASM>());

  //****************************** Crossfades ******************************//
  m_oscA_selfmix
      = NlToolbox::Crossfades::bipolarCrossFade(oscSampleA, shaperSampleA, signals.get<Signals::OSC_A_PMSSH>());
  m_oscA_crossmix
      = NlToolbox::Crossfades::bipolarCrossFade(oscSampleA, shaperSampleA, signals.get<Signals::OSC_B_PMASH>());

  m_oscB_selfmix
      = NlToolbox::Crossfades::bipolarCrossFade(oscSampleB, shaperSampleB, signals.get<Signals::OSC_B_PMSSH>());
  m_oscB_crossmix
      = NlToolbox::Crossfades::bipolarCrossFade(oscSampleB, shaperSampleB, signals.get<Signals::OSC_A_PMBSH>());

  m_out_A = NlToolbox::Crossfades::bipolarCrossFade(oscSampleA, shaperSampleA, signals.get<Signals::SHP_A_MIX>());
  m_out_B = NlToolbox::Crossfades::bipolarCrossFade(oscSampleB, shaperSampleB, signals.get<Signals::SHP_B_MIX>());

  //******************* Envelope Influence (Magnitudes) ********************//
  m_out_A *= signals.get<Signals::ENV_A_MAG>();
  m_out_B *= signals.get<Signals::ENV_B_MAG>();

  //**************************** Feedback Mix ******************************//
  tmpVar = _feedbackSample * signals.get<Signals::SHP_A_FBEC>();
  m_out_A = NlToolbox::Crossfades::unipolarCrossFade(m_out_A, tmpVar, signals.get<Signals::SHP_A_FBM>());

  tmpVar = _feedbackSample * signals.get<Signals::SHP_B_FBEC>();
  m_out_B = NlToolbox::Crossfades::unipolarCrossFade(m_out_B, tmpVar, signals.get<Signals::SHP_B_FBM>());

  //************************** Ring Modulation *****************************//
  tmpVar = m_out_A * m_out_B;

  m_out_A = NlToolbox::Crossfades::unipolarCrossFade(m_out_A, tmpVar, signals.get<Signals::SHP_A_RM>());
  m_out_B = NlToolbox::Crossfades::unipolarCrossFade(m_out_B, tmpVar, signals.get<Signals::SHP_B_RM>());
}

/******************************************************************************/
/** @brief
*******************************************************************************/

void ae_soundgenerator::resetDSP()
{
  m_out_A = 0.f;
  m_out_B = 0.f;

  m_chiA_stateVar = 0.f;
  m_chiB_stateVar = 0.f;

  m_oscA_selfmix = 0.f;
  m_oscA_crossmix = 0.f;
  m_oscA_phase_stateVar = 0.f;

  m_oscB_selfmix = 0.f;
  m_oscB_crossmix = 0.f;
  m_oscB_phase_stateVar = 0.f;

  m_feedback_phase = 0.f;
}
