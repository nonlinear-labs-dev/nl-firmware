/******************************************************************************/
/** @file           ae_svfilter.h
    @date           2018-10-11
    @version        1.0
    @author         Anton Schmied
    @brief          Combfilter with FIR Filter Version and FM+F clipping
    @todo
*******************************************************************************/

#pragma once

#include "nltoolbox.h"
#include "dsp_defines_signallabels.h"

using namespace NlToolbox::Constants;


#include "pe_defines_config.h" /// for testing the clipping limits

struct ae_svfilter_fir
{
    ae_svfilter_fir();

    float m_out;
    float m_warpConst_2PI;

    void init(float _samplerate);
    void apply(float _sampleA, float _sampleB, float _sampleComb, ParameterStorage &params);
    void resetDSP();

    //**************************** State Variables ****************************//
    float m_first_fir_stateVar, m_second_fir_stateVar;
    float m_first_int1_stateVar, m_first_int2_stateVar;
    float m_second_int1_stateVar, m_second_int2_stateVar;

    float m_first_sv_sample;
    float m_first_sat_stateVar, m_second_sat_stateVar;
};
