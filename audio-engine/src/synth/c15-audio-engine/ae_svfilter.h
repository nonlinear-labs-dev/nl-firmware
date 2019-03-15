/******************************************************************************/
/** @file           ae_svfilter.h
    @date           2018-07-05
    @version        1.0
    @author         Anton Schmied
    @brief          Combfilter with no FIR Filter Version and FM+F clipping
    @todo
*******************************************************************************/

#pragma once

#include "nltoolbox.h"
#include "dsp_defines_signallabels.h"

using namespace NlToolbox::Constants;

#include "pe_defines_config.h" /// for testing the clipping limits

struct ae_svfilter
{
    ae_svfilter();      // Default Constructor

    float m_out;
    float m_warpConst_2PI;

    void init(float _samplerate);
    void apply(float _sampleA, float _sampleB, float _sampleComb, float *_signal);
    void resetDSP();

    //**************************** State Variables ****************************//
    float m_first_int1_stateVar, m_first_int2_stateVar;
    float m_second_int1_stateVar, m_second_int2_stateVar;

    float m_first_sv_sample;
    float m_first_sat_stateVar, m_second_sat_stateVar;
};
