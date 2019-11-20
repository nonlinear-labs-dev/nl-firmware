#include "ae_poly_fb_mix.h"

/******************************************************************************/
/** @file       ae_poly_fb_mix.cpp
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      feedback mixer
    @todo
*******************************************************************************/

// todo: FX I/II and Osc Cross handling

Engine::PolyFeedbackMixer::PolyFeedbackMixer()
{
}

void Engine::PolyFeedbackMixer::init(const float _samplerate)
{
  m_warpConst_PI = NlToolbox::Constants::pi / _samplerate;
  m_freqClip_min = _samplerate / 24000.0f;
  m_freqClip_max = _samplerate / 3.0f;
  m_hp30hz_b0 = (NlToolbox::Constants::twopi / _samplerate) * 30.0f;
  m_hp30hz_b0 = std::min(m_hp30hz_b0, 0.8f);
  m_out = m_hp30hz_stateVar = m_hp_b0 = m_hp_b1 = m_hp_a1 = m_hp_stateVar_1 = m_hp_stateVar_2 = 0.0f;
}

void Engine::PolyFeedbackMixer::set(PolySignals &_signals, const uint32_t _voiceId)
{
  auto omega
      = std::clamp(_signals.get(C15::Signals::Truepoly_Signals::FB_Mix_HPF)[_voiceId], m_freqClip_min, m_freqClip_max);
  omega = NlToolbox::Math::tan(omega * m_warpConst_PI);
  m_hp_a1[_voiceId] = (1.0f - omega) / (1.0f + omega);
  m_hp_b0[_voiceId] = 1.0f / (1.0f + omega);
  m_hp_b1[_voiceId] = (1.0f / (1.0f + omega)) * -1.0f;
}

void Engine::PolyFeedbackMixer::apply(const PolyValue &_sampleComb, const PolyValue &_sampleSVF,
                                      const PolyValue &_sampleFX, PolySignals &_signals)
{
  // sampleFx: PolyValue?
  auto tmpVar = _sampleFX * _signals.get(C15::Signals::Quasipoly_Signals::FB_Mix_FX)
      + _sampleComb * _signals.get(C15::Signals::Quasipoly_Signals::FB_Mix_Comb)
      + _sampleSVF * _signals.get(C15::Signals::Quasipoly_Signals::FB_Mix_SVF);
  m_out = m_hp_b0 * tmpVar;  // HP
  m_out += m_hp_b1 * m_hp_stateVar_1;
  m_out += m_hp_a1 * m_hp_stateVar_2;
  m_hp_stateVar_1 = tmpVar + NlToolbox::Constants::DNC_const;
  m_hp_stateVar_2 = m_out + NlToolbox::Constants::DNC_const;
  m_out *= _signals.get(C15::Signals::Quasipoly_Signals::FB_Mix_Drive);
  tmpVar = m_out;
  m_out = sinP3_wrap(m_out);
  m_out = threeRanges(m_out, tmpVar, _signals.get(C15::Signals::Quasipoly_Signals::FB_Mix_Fold));
  tmpVar = m_out * m_out;
  tmpVar -= m_hp30hz_stateVar;  // HP 30Hz
  m_hp30hz_stateVar = tmpVar * m_hp30hz_b0 + m_hp30hz_stateVar + NlToolbox::Constants::DNC_const;
  m_out = parAsym(m_out, tmpVar, _signals.get(C15::Signals::Quasipoly_Signals::FB_Mix_Asym));
  m_out = m_out * _signals.get(C15::Signals::Truepoly_Signals::FB_Mix_Lvl);
}

void Engine::PolyFeedbackMixer::resetDSP()
{
  m_out = m_hp30hz_stateVar = m_hp_stateVar_1 = m_hp_stateVar_2 = 0.0f;
}
