/******************************************************************************/
/** @file           ae_outputmixer.h
    @date           2018-05-09
    @version        1.0
    @author         Anton Schmied
    @brief          Outputmixer
    @todo           Filter auflösen!
*******************************************************************************/

#pragma once

#include "nltoolbox.h"
#include "dsp_defines_signallabels.h"
#include <vector>

class SignalStorage;
using namespace NlToolbox::Constants;

struct ae_outputmixer
{
  ae_outputmixer();  // Default Constructor

  float m_out_L, m_out_R;

  float m_hp30hz_b0;

  std::vector<float> m_hp30hz_stateVar_L;
  std::vector<float> m_hp30hz_stateVar_R;

  void init(float _samplerate, uint32_t _numOfVoices);
  void combine(float _sampleA, float _sampleB, float _sampleComb, float _sampleSVFilter, SignalStorage &params,
               uint32_t _voiceID);
  void filter_level(SignalStorage &params);
  void resetDSP();

  //*************************** Highpass Filters ****************************//
  float m_hp_b0, m_hp_b1, m_hp_a1;

  float m_hp_stateVar_L1, m_hp_stateVar_R1;
  float m_hp_stateVar_L2, m_hp_stateVar_R2;
};
