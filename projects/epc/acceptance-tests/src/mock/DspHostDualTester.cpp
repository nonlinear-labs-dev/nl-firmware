#include "DspHostDualTester.h"

DspHostDualTester::DspHostDualTester(dsp_host_dual* _host)
    : m_host{ _host }
{
}

uint8_t DspHostDualTester::getActiveVoices(const VoiceGroup _group)
{
  // retrieving the polyphonic Gate Envelope signal from both Poly Sections/Parts (12 Voices per Part)
  // (per Voice: Gate Signal is 1.0 (immediately) after KeyPress, and (almost immediately) 0.0 after KeyRelease)

  const uint32_t gateSignalIndex = static_cast<uint32_t>(C15::Signals::Truepoly_Signals::Env_G_Sig);
  float gateSignal;

  // obtain specific Part if desired, otherwise both Parts
  switch(_group)
  {
    case VoiceGroup::I:
      gateSignal = sumUp(m_host->m_poly[0].m_signals.get_poly(gateSignalIndex));
      break;
    case VoiceGroup::II:
      gateSignal = sumUp(m_host->m_poly[1].m_signals.get_poly(gateSignalIndex));
      break;
    default:
      gateSignal = sumUp(m_host->m_poly[0].m_signals.get_poly(gateSignalIndex))
          + sumUp(m_host->m_poly[1].m_signals.get_poly(gateSignalIndex));
      break;
  }

  // having summed up all voices, we obtain the number of "active" Voices (0 ... 24)
  // the obtained value should be sample-precise and is not dependant from Sustain or Level Parameters

  return static_cast<uint8_t>(gateSignal);  // 0 : no voice active / 1, 2, ... 23, 24 : n voices active
}
