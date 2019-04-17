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
    m_boundVoice = m_sigIdsignaldata[v];
    return *this;
  }

  inline const float &get(Signals sigId) const
  {
    return m_boundVoice[static_cast<uint32_t>(sigId)];
  }

  inline const float &get(uint32_t voice, uint32_t sigId) const
  {
    return m_sigIdsignaldata[voice][sigId];
  }

  inline void set(Signals sigId, float value)
  {
    m_boundVoice[static_cast<uint32_t>(sigId)] = value;
  }

  inline void set(Signals sigId, uint32_t voice, float value)
  {
    m_sigIdsignaldata[voice][static_cast<uint32_t>(sigId)] = value;
  }

 private:
  float m_sigIdsignaldata[dsp_number_of_voices][sig_number_of_signal_items] = {};
  float *m_boundVoice = m_sigIdsignaldata[0];
};
