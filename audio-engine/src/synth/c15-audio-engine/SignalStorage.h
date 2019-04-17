#pragma once

#include "pe_defines_config.h"
#include "dsp_defines_signallabels.h"

#include <stdlib.h>

class SignalStorage
{
 public:
  SignalStorage() = default;

  inline SignalStorage &bindToVoice(uint32_t v)
  {
    m_boundVoice = m_paramsignaldata[v];
    return *this;
  }

  inline const float &get(Signals param) const
  {
    return m_boundVoice[static_cast<uint32_t>(param)];
  }

  inline const float &get(uint32_t voice, uint32_t param) const
  {
    return m_paramsignaldata[voice][param];
  }

  inline void set(Signals param, float value)
  {
    m_boundVoice[static_cast<uint32_t>(param)] = value;
  }

  inline void set(Signals param, uint32_t voice, float value)
  {
    m_paramsignaldata[voice][static_cast<uint32_t>(param)] = value;
  }

 private:
  float m_paramsignaldata[dsp_number_of_voices][sig_number_of_signal_items] = {};
  float *m_boundVoice = m_paramsignaldata[0];
};
