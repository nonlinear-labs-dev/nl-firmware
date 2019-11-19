#include "ae_poly_section.h"

/******************************************************************************/
/** @file       ae_poly_section.cpp
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new container for all polyphonic parameters and dsp
    @todo
*******************************************************************************/

PolySection::PolySection()
{
}

void PolySection::add_copy_audio_id(const uint32_t _smootherId, const uint32_t _signalId)
{
  m_smoothers.m_copy_audio.add_copy_id(_smootherId, _signalId);
}

void PolySection::add_copy_fast_id(const uint32_t _smootherId, const uint32_t _signalId)
{
  m_smoothers.m_copy_fast.add_copy_id(_smootherId, _signalId);
}

void PolySection::add_copy_slow_id(const uint32_t _smootherId, const uint32_t _signalId)
{
  m_smoothers.m_copy_slow.add_copy_id(_smootherId, _signalId);
}

void PolySection::start_sync(const uint32_t _id, const float _dest)
{
  m_smoothers.start_sync(_id, _dest);
}

void PolySection::start_audio(const uint32_t _id, const float _dx, const float _dest)
{
  m_smoothers.start_audio(_id, _dx, _dest);
}

void PolySection::start_fast(const uint32_t _id, const float _dx, const float _dest)
{
  m_smoothers.start_fast(_id, _dx, _dest);
}

void PolySection::start_slow(const uint32_t _id, const float _dx, const float _dest)
{
  m_smoothers.start_slow(_id, _dx, _dest);
}

void PolySection::render_audio(const float _mute)
{
  m_smoothers.render_audio();
  auto traversal = &m_smoothers.m_copy_audio;
  for(uint32_t i = 0; i < traversal->m_length; i++)
  {
    m_signals.set_mono(traversal->m_signalId[i], m_smoothers.get_audio(traversal->m_smootherId[i]));
  }
  for(uint32_t v = 0; v < m_voices; v++)
  {
    postProcess_audio(v);
  }
}

void PolySection::render_fast()
{
  m_smoothers.render_fast();
  auto traversal = &m_smoothers.m_copy_fast;
  for(uint32_t i = 0; i < traversal->m_length; i++)
  {
    m_signals.set_mono(traversal->m_signalId[i], m_smoothers.get_fast(traversal->m_smootherId[i]));
  }
  for(uint32_t v = 0; v < m_voices; v++)
  {
    postProcess_fast(v);
  }
}

void PolySection::render_slow()
{
  m_smoothers.render_slow();
  auto traversal = &m_smoothers.m_copy_slow;
  for(uint32_t i = 0; i < traversal->m_length; i++)
  {
    m_signals.set_mono(traversal->m_signalId[i], m_smoothers.get_slow(traversal->m_smootherId[i]));
  }
  for(uint32_t v = 0; v < m_voices; v++)
  {
    postProcess_slow(v);
  }
}

void PolySection::keyDown(const uint32_t _voiceId, const uint32_t _unisonVoices, const uint32_t _unisonIndex,
                          const bool _stolen, const float _tune, const float _vel)
{
  postProcess_key(_voiceId);
}

void PolySection::keyUp(const uint32_t _voiceId, const float _tune, const float _vel)
{
}

void PolySection::postProcess_audio(const uint32_t _voiceId)
{
}

void PolySection::postProcess_fast(const uint32_t _voiceId)
{
}

void PolySection::postProcess_slow(const uint32_t _voiceId)
{
}

void PolySection::postProcess_key(const uint32_t _voiceId)
{
}
