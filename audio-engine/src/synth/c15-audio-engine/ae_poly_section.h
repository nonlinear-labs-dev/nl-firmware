#pragma once

/******************************************************************************/
/** @file       ae_poly_section.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new container for all polyphonic parameters and dsp
    @todo
*******************************************************************************/

#include "smoother_handle.h"
#include "unison_spread_table.h"
#include "pe_exponentiator.h"
#include "ae_info.h"
#include "ae_envelopes.h"
#include "ae_poly_soundgen.h"

class PolySection
{
 public:
  PolySignals m_signals;
  // feedback poly values:
  PolyValue *m_fb_osc_a, *m_fb_osc_b;
  // output poly values:
  PolyValue m_osc_a = {}, m_osc_b = {};
  // feedback mono values:
  float *m_fb0_dry, *m_fb0_wet, *m_fb1_dry, *m_fb1_wet, *m_reference;
  // output mono values:
  float m_send0_l = 0.0f, m_send0_r = 0.0f, m_send1_l = 0.0f, m_send1_r = 0.0f, m_note_shift = 0.0f,
        m_millisecond = 0.0f;
  uint32_t m_uVoice = 0, m_key_active = 0;
  PolySection();
  void init(exponentiator *_convert, PolyValue *_fb_osc_a, PolyValue *_fb_osc_b, float *_fb0_dry, float *_fb0_wet,
            float *_fb1_dry, float *_fb1_wet, float *_reference, const float _ms, const float _gateRelease,
            const float _samplerate);
  void add_copy_audio_id(const uint32_t _smootherId, const uint32_t _signalId);
  void add_copy_fast_id(const uint32_t _smootherId, const uint32_t _signalId);
  void add_copy_slow_id(const uint32_t _smootherId, const uint32_t _signalId);
  void start_sync(const uint32_t _id, const float _dest);
  void start_audio(const uint32_t _id, const float _dx, const float _dest);
  void start_fast(const uint32_t _id, const float _dx, const float _dest);
  void start_slow(const uint32_t _id, const float _dx, const float _dest);
  void render_audio(const float _mute);
  void render_fast();
  void render_slow();
  void keyDown(const uint32_t _voiceId, const uint32_t _unisonIndex, const bool _stolen, const float _tune,
               const float _vel);
  void keyUp(const uint32_t _voiceId, const uint32_t _unisonIndex, const float _tune, const float _vel);
  void resetEnvelopes();

 private:
  SmootherHandle<C15::Smoothers::Poly_Sync, C15::Smoothers::Poly_Audio, C15::Smoothers::Poly_Fast,
                 C15::Smoothers::Poly_Slow>
      m_smoothers;
  exponentiator *m_convert;
  UnisonSpreadTable<C15::Config::total_polyphony> m_spread;
  Engine::Envelopes::SplitEnvelope<C15::Config::local_polyphony> m_env_a, m_env_b;
  Engine::Envelopes::RetriggerEnvelope<C15::Config::local_polyphony> m_env_c;
  Engine::Envelopes::GateEnvelope<C15::Config::local_polyphony> m_env_g;
  Engine::PolySoundGenerator m_soundgenerator;
  float m_shift[C15::Config::local_polyphony] = {}, m_key_tune[C15::Config::local_polyphony] = {}, m_nyquist;
  uint32_t m_unison_index[C15::Config::local_polyphony] = {};
  const uint32_t m_voices = C15::Config::local_polyphony;
  float evalNyquist(const float _value);
  void postProcess_audio(const uint32_t _voiceId, const float _mute);
  void postProcess_fast(const uint32_t _voiceId);
  void postProcess_slow(const uint32_t _voiceId);
  void postProcess_key(const uint32_t _voiceId);
  void startEnvelopes(const uint32_t _voiceId, const float _pitch, const float _vel);
  void stopEnvelopes(const uint32_t _voiceId, const float _pitch, const float _vel);
  void updateEnvLevels(const uint32_t _voiceId);
  void updateEnvTimes(const uint32_t _voiceId);
  void setSlowFilterCoefs(const uint32_t _voiceId);
};
