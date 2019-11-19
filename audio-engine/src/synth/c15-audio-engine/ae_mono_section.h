#pragma once

/******************************************************************************/
/** @file       ae_mono_section.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new container for all monophonic parameters and dsp
    @todo
*******************************************************************************/

#include "smoother_handle.h"
#include "ae_info.h"

class MonoSection
{
 public:
  float m_out_l = 0.0f, m_out_r = 0.0f, m_dry = 0.0f, m_wet = 0.0f;
  MonoSection();
  void add_copy_audio_id(const uint32_t _smootherId, const uint32_t _signalId);
  void add_copy_fast_id(const uint32_t _smootherId, const uint32_t _signalId);
  void add_copy_slow_id(const uint32_t _smootherId, const uint32_t _signalId);
  void start_sync(const uint32_t _id, const float _dest);
  void start_audio(const uint32_t _id, const float _dx, const float _dest);
  void start_fast(const uint32_t _id, const float _dx, const float _dest);
  void start_slow(const uint32_t _id, const float _dx, const float _dest);
  void render_audio(const float _left, const float _right);
  void render_fast();
  void render_slow();
  void keyDown(const float _vel);

 private:
  MonoSignals m_signals;
  SmootherHandle<C15::Smoothers::Mono_Sync, C15::Smoothers::Mono_Audio, C15::Smoothers::Mono_Fast,
                 C15::Smoothers::Mono_Slow>
      m_smoothers;
  void postProcess_audio();
  void postProcess_fast();
  void postProcess_slow();
};
