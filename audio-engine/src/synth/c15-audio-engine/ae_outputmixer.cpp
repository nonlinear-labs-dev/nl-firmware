/******************************************************************************/
/** @file           ae_outputmixer.cpp
    @date           2018-05-09
    @version        1.0
    @author         Anton Schmied
    @brief          Outputmixer
    @todo
*******************************************************************************/

#include "ae_outputmixer.h"

/******************************************************************************/
/** @brief
*******************************************************************************/

ae_outputmixer::ae_outputmixer()
{

}



/******************************************************************************/
/** @brief
*******************************************************************************/

void ae_outputmixer::init(float _samplerate, uint32_t _numOfVoices)
{
    m_out_L = 0.f;
    m_out_R = 0.f;

    //**************************** 30 Hz Highpass ****************************//
    m_hp30hz_b0 = (6.28319f / _samplerate) * 30.f;
    m_hp30hz_b0 = std::min(m_hp30hz_b0, 0.8f);

    m_hp30hz_stateVar_L.resize(_numOfVoices);
    std::fill(m_hp30hz_stateVar_L.begin(), m_hp30hz_stateVar_L.end(), 0.f);
    m_hp30hz_stateVar_R.resize(_numOfVoices);
    std::fill(m_hp30hz_stateVar_R.begin(), m_hp30hz_stateVar_R.end(), 0.f);


    //*************************** 1 pole Highpass ****************************//
    float omega = NlToolbox::Math::tan(12.978f * NlToolbox::Constants::pi / static_cast<float>(_samplerate));

    m_hp_a1 = (1.f - omega) / (1.f + omega);
    m_hp_b0 = 1.f / (1.f + omega);
    m_hp_b1 = (1.f / (1.f + omega)) * -1.f;

    m_hp_stateVar_L1 = 0.f;
    m_hp_stateVar_L2 = 0.f;
    m_hp_stateVar_R1 = 0.f;
    m_hp_stateVar_R2 = 0.f;
}



/******************************************************************************/
/** @brief
*******************************************************************************/

void ae_outputmixer::combine(float _sampleA, float _sampleB, float _sampleComb, float _sampleSVFilter, float *_signal, uint32_t _voiceID)
{
    //******************************* Left Mix *******************************//
    float mainSample = _signal[OUT_A_L] * _sampleA
                     + _signal[OUT_B_L] * _sampleB
                     + _signal[OUT_CMB_L] * _sampleComb
                     + _signal[OUT_SVF_L] * _sampleSVFilter;

    //************************* Left Sample Shaper ***************************//
    mainSample *= _signal[OUT_DRV];
    float tmpVar = mainSample;

    mainSample = NlToolbox::Math::sinP3_wrap(mainSample);
    mainSample = NlToolbox::Others::threeRanges(mainSample, tmpVar, _signal[OUT_FLD]);

    tmpVar = mainSample * mainSample;
    tmpVar = tmpVar - m_hp30hz_stateVar_L[_voiceID];
    m_hp30hz_stateVar_L[_voiceID] = tmpVar * m_hp30hz_b0 + m_hp30hz_stateVar_L[_voiceID] + DNC_const;

    mainSample = NlToolbox::Others::parAsym(mainSample, tmpVar, _signal[OUT_ASM]);

    m_out_L += mainSample;

    //****************************** Right Mix *******************************//
    mainSample = _signal[OUT_A_R] * _sampleA
               + _signal[OUT_B_R] * _sampleB
               + _signal[OUT_CMB_R] * _sampleComb
               + _signal[OUT_SVF_R] * _sampleSVFilter;

    //************************ Right Sample Shaper ***************************//
    mainSample *= _signal[OUT_DRV];
    tmpVar = mainSample;

    mainSample = NlToolbox::Math::sinP3_wrap(mainSample);
    mainSample = NlToolbox::Others::threeRanges(mainSample, tmpVar, _signal[OUT_FLD]);

    tmpVar = mainSample * mainSample;
    tmpVar = tmpVar - m_hp30hz_stateVar_R[_voiceID];
    m_hp30hz_stateVar_R[_voiceID] = tmpVar * m_hp30hz_b0 + m_hp30hz_stateVar_R[_voiceID] + DNC_const;

    mainSample = NlToolbox::Others::parAsym(mainSample, tmpVar, _signal[OUT_ASM]);

    m_out_R += mainSample;
}



/******************************************************************************/
/** @brief
*******************************************************************************/

void ae_outputmixer::filter_level(float *_signal)
{
    float tmpVar  = m_hp_b0 * m_out_L;          // HP L
    tmpVar += m_hp_b1 * m_hp_stateVar_L1;
    tmpVar += m_hp_a1 * m_hp_stateVar_L2;

    m_hp_stateVar_L1 = m_out_L + DNC_const;
    m_hp_stateVar_L2 = tmpVar + DNC_const;

    tmpVar  = m_hp_b0 * m_out_R;                // HP R
    tmpVar += m_hp_b1 * m_hp_stateVar_R1;
    tmpVar += m_hp_a1 * m_hp_stateVar_R2;

    m_hp_stateVar_R1 = m_out_R + DNC_const;
    m_hp_stateVar_R2 = tmpVar + DNC_const;


    m_out_L *= _signal[OUT_LVL];
    m_out_R *= _signal[OUT_LVL];
}

/******************************************************************************/
/** @brief
*******************************************************************************/

void ae_outputmixer::resetDSP()
{
    m_out_L = 0.f;
    m_out_R = 0.f;

    //**************************** 30 Hz Highpass ****************************//

    std::fill(m_hp30hz_stateVar_L.begin(), m_hp30hz_stateVar_L.end(), 0.f);
    std::fill(m_hp30hz_stateVar_R.begin(), m_hp30hz_stateVar_R.end(), 0.f);

    //*************************** 1 pole Highpass ****************************//

    m_hp_stateVar_L1 = 0.f;
    m_hp_stateVar_L2 = 0.f;
    m_hp_stateVar_R1 = 0.f;
    m_hp_stateVar_R2 = 0.f;
}
