/******************************************************************************/
/** @file		feedbackmixer.h
    @date		2016-07-30
    @version	1.0
    @author		Anton Schmied
    @brief		Feedback Mixer
*******************************************************************************/

#pragma once

#include "dsp_defines_signallabels.h"
#include "nltoolbox.h"

class SignalStorage;
using namespace NlToolbox::Constants;

struct ae_feedbackmixer
{
  ae_feedbackmixer();  // Default Constructor

  void init(float _samplerate);
  void set(SignalStorage &signals);
  void apply(float _sampleComb, float _sampleSVF, float _sampleFX, SignalStorage &signals);
  void resetDSP();

  float m_out;

  float m_warpConst_PI;
  float m_freqClip_min;
  float m_freqClip_max;

  //**************************** 30Hz Highpass *****************************//
  float m_hp30hz_b0;
  float m_hp30hz_stateVar;

  //*************************** 1 pole Highpass ****************************//
  float m_hp_b0, m_hp_b1, m_hp_a1;

  float m_hp_stateVar_1, m_hp_stateVar_2;
};
