#pragma once

#include "pe_defines_config.h"
#include "dsp_defines_signallabels.h"

#include <stdlib.h>

class SignalStorage
{
 public:
  SignalStorage() = default;

  inline float &operator[](SignalLabel param)
  {
    return m_boundVoice[static_cast<uint32_t>(param)];
  }

  inline SignalStorage &bindToVoice(uint32_t v)
  {
    m_boundVoice = m_paramsignaldata[v];
    return *this;
  }

  inline float get(uint32_t voice, uint32_t param)
  {
    return m_paramsignaldata[voice][param];
  }

 private:
  float m_paramsignaldata[dsp_number_of_voices][sig_number_of_signal_items] = {};
  float *m_boundVoice = m_paramsignaldata[0];
};
