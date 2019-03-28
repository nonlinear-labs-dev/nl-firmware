#pragma once

#include "pe_defines_config.h"
#include <stdlib.h>

class ParameterStorage
{
 public:
  ParameterStorage() = default;

  inline float &operator[](int param)
  {
    return m_paramsignaldata[m_boundVoice][param];
  }

  ParameterStorage &bindToVoice(int v)
  {
    m_boundVoice = v;
  }

  float get(int voice, int param)
  {
    return m_paramsignaldata[voice][param];
  }

 private:
  int m_boundVoice = 0;
  float m_paramsignaldata[dsp_number_of_voices][sig_number_of_signal_items] = {};
};
