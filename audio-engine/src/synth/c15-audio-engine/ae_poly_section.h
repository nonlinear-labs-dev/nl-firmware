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
#include "ae_info.h"

class PolySection
{
 public:
  // feedback poly values:
  PolyValue *m_fb_osc_a, *m_fb_osc_b;
  // output poly values:
  PolyValue m_osc_a = {}, m_osc_b = {};
  // feedback mono values:
  float *m_fb0_dry, *m_fb0_wet, *m_fb1_dry, *m_fb1_wet;
  // output mono values:
  float m_send0_l = 0.0f, m_send0_r = 0.0f, m_send1_l = 0.0f, m_send1_r = 0.0f;
  PolySection();
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
  void keyUp(const uint32_t _voiceId, const float _tune, const float _vel);

 private:
  PolySignals m_signals;
  SmootherHandle<C15::Smoothers::Poly_Sync, C15::Smoothers::Poly_Audio, C15::Smoothers::Poly_Fast,
                 C15::Smoothers::Poly_Slow>
      m_smoothers;
  void postProcess_audio();
  void postProcess_fast();
  void postProcess_slow();
};
