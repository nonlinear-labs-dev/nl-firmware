/******************************************************************************/
/** @file           ae_svfilter.h
    @date           2018-07-05
    @version        1.0
    @author         Anton Schmied
    @brief          Combfilter with FIR Filter Version, FM and F are clipped with
                    1.9 and only F is used for attenuation calculation
    @todo
*******************************************************************************/

#pragma once

#include "nltoolbox.h"
#include "dsp_defines_signallabels.h"

using namespace NlToolbox::Constants;

#include "pe_defines_config.h" /// for testing the clipping limits

class ae_svfilter_proto
{
public:
    ae_svfilter_proto();

    float m_out;
    float m_warpConst_2PI;

    void init(float _samplerate);
    void apply(float _sampleA, float _sampleB, float _sampleComb, float *_signal);
    void resetDSP();

    //**************************** State Variables ****************************//
    float m_first_fir_stateVar, m_second_fir_stateVar;
    float m_first_int1_stateVar, m_first_int2_stateVar;
    float m_second_int1_stateVar, m_second_int2_stateVar;

    float m_first_sv_sample;
    float m_first_sat_stateVar, m_second_sat_stateVar;
};
