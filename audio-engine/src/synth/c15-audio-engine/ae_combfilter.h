/******************************************************************************/
/** @file           ae_combfilter.h
    @date           2018-05-10
    @version        1.0
    @author         Anton Schmied
    @brief          Combfilter
    @todo
*******************************************************************************/

#pragma once

#include <vector>
#include "nltoolbox.h"
#include "dsp_defines_signallabels.h"

using namespace NlToolbox::Constants;

#define COMB_BUFFER_SIZE    8192

struct ae_combfilter
{
    ae_combfilter();        // Default Contructor

    float m_out;     // Generated Sample
    float m_decayStateVar;

    float m_sampleInterval;
    float m_warpConst_PI;
    float m_warpConst_2PI;
    float m_freqClip_2;
    float m_freqClip_4;
    float m_freqClip_24576;

    void init(float _samplerate, uint32_t _upsampleFactor);
    void apply(float _sampleA, float _sampleB, float *_signal);
    void set(float *_signal, float _samplerate);
    void setDelaySmoother();
    void resetDSP();

    //**************************** Highpass Filter ****************************//
    float m_hpCoeff_b0, m_hpCoeff_b1, m_hpCoeff_a1;
    float m_hpInStateVar, m_hpOutStateVar;

    //***************************** Lowpass Filter ****************************//
    float m_lpCoeff;
    float m_lpStateVar;

    //***************************** Allpass Filter ****************************//
    float m_apCoeff_1, m_apCoeff_2;
    float m_apStateVar_1;        // Allpass State Variables
    float m_apStateVar_2;
    float m_apStateVar_3;
    float m_apStateVar_4;

    //****************************** Delay/ Decay *****************************//
    int32_t m_buffer_indx;
    int32_t m_buffer_sz_m1;
    std::vector<float> m_buffer;

    float m_delaySamples;
    float m_delayFreqClip;
    float m_delayConst;
    float m_delayStateVar;

    float m_decayGain;
};
