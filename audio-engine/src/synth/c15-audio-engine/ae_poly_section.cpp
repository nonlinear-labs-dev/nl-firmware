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

void PolySection::init(exponentiator *_convert, PolyValue *_fb_osc_a, PolyValue *_fb_osc_b, float *_fb0_dry,
                       float *_fb0_wet, float *_fb1_dry, float *_fb1_wet, const float _ms, const float _gateRelease)
{
  m_convert = _convert;
  m_fb_osc_a = _fb_osc_a;
  m_fb_osc_b = _fb_osc_b;
  m_fb0_dry = _fb0_dry;
  m_fb0_wet = _fb0_wet;
  m_fb1_dry = _fb1_dry;
  m_fb1_wet = _fb1_wet;
  m_millisecond = _ms;
  m_env_g.init(_ms * _gateRelease);
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
    postProcess_audio(v, _mute);
  }
  // todo: main poly audio dsp (makepolysound)
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
  // todo: poly fast triggers
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
  // todo: poly slow triggers
}

void PolySection::keyDown(const uint32_t _voiceId, const uint32_t _unisonIndex, const bool _stolen, const float _tune,
                          const float _vel)
{
  const float noteShift = m_shift[_voiceId] = m_note_shift,
              unisonDetune = m_smoothers.get(C15::Smoothers::Poly_Slow::Unison_Detune),
              masterTune = m_smoothers.get(C15::Smoothers::Poly_Slow::Voice_Grp_Tune);
  m_unison_index[_voiceId] = _unisonIndex;
  const float notePitch = _tune + (unisonDetune * m_spread.m_detune[m_uVoice][_unisonIndex]) + masterTune + noteShift;
  postProcess_key(_voiceId);
  // todo: setPolySlowFilterCoeffs(m_parameters, voicePos = _voiceId); m_combfilter.setDelaySmoother(voicePos);
  if(_stolen)
  {
    // todo: noteSteal?
  }
  // todo: m_soundgenerator.resetPhase(startPhase = 0.0f, voicePos);
  startEnvelopes(_voiceId, notePitch, _vel);
  m_key_active++;
}

void PolySection::keyUp(const uint32_t _voiceId, const uint32_t _unisonIndex, const float _tune, const float _vel)
{
  const float noteShift = m_shift[_voiceId], unisonDetune = m_smoothers.get(C15::Smoothers::Poly_Slow::Unison_Detune),
              masterTune = m_smoothers.get(C15::Smoothers::Poly_Slow::Voice_Grp_Tune);
  const float notePitch = _tune + (unisonDetune * m_spread.m_detune[m_uVoice][_unisonIndex]) + masterTune + noteShift;
  stopEnvelopes(_voiceId, notePitch, _vel);
  m_key_active--;
}

void PolySection::resetEnvelopes()
{
  m_env_a.reset();
  m_env_b.reset();
  m_env_c.reset();
  m_env_g.reset();
}

void PolySection::postProcess_audio(const uint32_t _voiceId, const float _mute)
{
  m_env_a.tick(_voiceId, _mute);
  m_env_b.tick(_voiceId, _mute);
  m_env_c.tick(_voiceId, _mute);
  m_env_g.tick(_voiceId);
  float gain = m_smoothers.get(C15::Smoothers::Poly_Fast::Env_A_Gain);
  m_signals.set_poly(C15::Signals::Truepoly_Signals::Env_A_Mag, _voiceId,
                     gain * m_env_a.m_body[_voiceId].m_signal_magnitude);
  m_signals.set_poly(C15::Signals::Truepoly_Signals::Env_A_Tmb, _voiceId,
                     gain * m_env_a.m_body[_voiceId].m_signal_timbre);
  gain = m_smoothers.get(C15::Smoothers::Poly_Fast::Env_B_Gain);
  m_signals.set_poly(C15::Signals::Truepoly_Signals::Env_B_Mag, _voiceId,
                     gain * m_env_b.m_body[_voiceId].m_signal_magnitude);
  m_signals.set_poly(C15::Signals::Truepoly_Signals::Env_B_Tmb, _voiceId,
                     gain * m_env_b.m_body[_voiceId].m_signal_timbre);
  m_signals.set_poly(C15::Signals::Truepoly_Signals::Env_C_Clip, _voiceId, m_env_c.m_body[_voiceId].m_signal_magnitude);
  m_signals.set_poly(C15::Signals::Truepoly_Signals::Env_C_Uncl, _voiceId,
                     m_env_c.m_body[_voiceId].m_signal_magnitude * m_env_c.m_clipFactor[_voiceId]);
  m_signals.set_poly(C15::Signals::Truepoly_Signals::Env_G_Sig, _voiceId, m_env_g.m_body[_voiceId].m_signal_magnitude);
  // todo: provide truepoly unison phase signal (parameter list), remaining audio post processing
  // todo: remainig audio poly post processing
}

void PolySection::postProcess_fast(const uint32_t _voiceId)
{
  updateEnvLevels(_voiceId);
  // todo: remaining fast poly post processing
}

void PolySection::postProcess_slow(const uint32_t _voiceId)
{
  updateEnvTimes(_voiceId);
  // todo: remaining slow poly post processing
}

void PolySection::postProcess_key(const uint32_t _voiceId)
{
  // todo: key post processing
}

void PolySection::startEnvelopes(const uint32_t _voiceId, const float _pitch, const float _vel)
{
  float time, timeKT, dest, levelVel, attackVel, decay1Vel, decay2Vel, levelKT, peak, unclipped;
  // env a
  timeKT = -0.5f * m_smoothers.get(C15::Smoothers::Poly_Sync::Env_A_Time_KT) * _pitch;
  levelVel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_A_Lvl_Vel);
  attackVel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_A_Att_Vel) * _vel;
  decay1Vel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_A_Dec_1_Vel) * _vel;
  decay2Vel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_A_Dec_2_Vel) * _vel;
  levelKT = m_smoothers.get(C15::Smoothers::Poly_Sync::Env_A_Lvl_KT) * _pitch;
  peak = std::min(m_convert->eval_level(((1.0f - _vel) * levelVel) + levelKT), env_clip_peak);
  m_env_a.m_levelFactor[_voiceId] = peak;
  m_env_a.m_timeFactor[_voiceId][0] = m_convert->eval_level(timeKT + attackVel) * m_millisecond;
  m_env_a.m_timeFactor[_voiceId][1] = m_convert->eval_level(timeKT + decay1Vel) * m_millisecond;
  m_env_a.m_timeFactor[_voiceId][2] = m_convert->eval_level(timeKT + decay2Vel) * m_millisecond;
  m_env_a.setSplitValue(m_smoothers.get(C15::Smoothers::Poly_Sync::Env_A_Split));
  m_env_a.setAttackCurve(m_smoothers.get(C15::Smoothers::Poly_Sync::Env_A_Att_Curve));
  m_env_a.setPeakLevel(_voiceId, peak);
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_A_Att) * m_env_a.m_timeFactor[_voiceId][0];
  m_env_a.setSegmentDx(_voiceId, 1, 1.0f / (time + 1.0f));
  m_env_a.setSegmentDest(_voiceId, 1, false, peak);
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_A_Dec_1) * m_env_a.m_timeFactor[_voiceId][1];
  m_env_a.setSegmentDx(_voiceId, 2, 1.0f / (time + 1.0f));
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_A_BP);
  m_env_a.setSegmentDest(_voiceId, 2, true, peak);
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_A_Dec_2) * m_env_a.m_timeFactor[_voiceId][2];
  m_env_a.setSegmentDx(_voiceId, 3, 1.0f / (time + 1.0f));
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_A_Sus);
  m_env_a.setSegmentDest(_voiceId, 3, true, peak);
  // env b
  timeKT = -0.5f * m_smoothers.get(C15::Smoothers::Poly_Sync::Env_B_Time_KT) * _pitch;
  levelVel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_B_Lvl_Vel);
  attackVel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_B_Att_Vel) * _vel;
  decay1Vel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_B_Dec_1_Vel) * _vel;
  decay2Vel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_B_Dec_2_Vel) * _vel;
  levelKT = m_smoothers.get(C15::Smoothers::Poly_Sync::Env_B_Lvl_KT) * _pitch;
  peak = std::min(m_convert->eval_level(((1.0f - _vel) * levelVel) + levelKT), env_clip_peak);
  m_env_b.m_levelFactor[_voiceId] = peak;
  m_env_b.m_timeFactor[_voiceId][0] = m_convert->eval_level(timeKT + attackVel) * m_millisecond;
  m_env_b.m_timeFactor[_voiceId][1] = m_convert->eval_level(timeKT + decay1Vel) * m_millisecond;
  m_env_b.m_timeFactor[_voiceId][2] = m_convert->eval_level(timeKT + decay2Vel) * m_millisecond;
  m_env_b.setSplitValue(m_smoothers.get(C15::Smoothers::Poly_Sync::Env_B_Split));
  m_env_b.setAttackCurve(m_smoothers.get(C15::Smoothers::Poly_Sync::Env_B_Att_Curve));
  m_env_b.setPeakLevel(_voiceId, peak);
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_B_Att) * m_env_b.m_timeFactor[_voiceId][0];
  m_env_b.setSegmentDx(_voiceId, 1, 1.0f / (time + 1.0f));
  m_env_b.setSegmentDest(_voiceId, 1, false, peak);
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_B_Dec_1) * m_env_b.m_timeFactor[_voiceId][1];
  m_env_b.setSegmentDx(_voiceId, 2, 1.0f / (time + 1.0f));
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_B_BP);
  m_env_b.setSegmentDest(_voiceId, 2, true, peak);
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_B_Dec_2) * m_env_b.m_timeFactor[_voiceId][2];
  m_env_b.setSegmentDx(_voiceId, 3, 1.0f / (time + 1.0f));
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_B_Sus);
  m_env_b.setSegmentDest(_voiceId, 3, true, peak);
  // env c
  timeKT = -0.5f * m_smoothers.get(C15::Smoothers::Poly_Sync::Env_C_Time_KT) * _pitch;
  levelVel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_C_Lvl_Vel);
  attackVel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_C_Att_Vel) * _vel;
  levelKT = m_smoothers.get(C15::Smoothers::Poly_Sync::Env_C_Lvl_KT) * _pitch;
  unclipped = m_convert->eval_level(((1.0f - _vel) * levelVel) + levelKT);
  peak = std::min(unclipped, env_clip_peak);
  m_env_c.m_clipFactor[_voiceId] = unclipped / peak;
  m_env_c.m_levelFactor[_voiceId] = peak;
  m_env_c.m_timeFactor[_voiceId][0] = m_convert->eval_level(timeKT + attackVel) * m_millisecond;
  m_env_c.m_timeFactor[_voiceId][1] = m_convert->eval_level(timeKT + decay1Vel) * m_millisecond;
  m_env_c.m_timeFactor[_voiceId][2] = m_convert->eval_level(timeKT + decay2Vel) * m_millisecond;
  m_env_c.setAttackCurve(m_smoothers.get(C15::Smoothers::Poly_Sync::Env_C_Att_Curve));
  m_env_c.setRetriggerHardness(m_smoothers.get(C15::Smoothers::Poly_Sync::Env_C_Retr_H));
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_C_Att) * m_env_c.m_timeFactor[_voiceId][0];
  m_env_c.setSegmentDx(_voiceId, 1, 1.0f / (time + 1.0f));
  m_env_c.setSegmentDest(_voiceId, 1, peak);
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_C_Dec_1) * m_env_c.m_timeFactor[_voiceId][1];
  m_env_c.setSegmentDx(_voiceId, 2, 1.0f / (time + 1.0f));
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_C_BP);
  m_env_c.setSegmentDest(_voiceId, 2, peak);
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_C_Dec_2) * m_env_c.m_timeFactor[_voiceId][2];
  m_env_c.setSegmentDx(_voiceId, 3, 1.0f / (time + 1.0f));
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_C_Sus);
  m_env_c.setSegmentDest(_voiceId, 3, peak);
  // start envelopes
  m_env_a.start(_voiceId);
  m_env_b.start(_voiceId);
  m_env_c.start(_voiceId);
  m_env_g.start(_voiceId);
}

void PolySection::stopEnvelopes(const uint32_t _voiceId, const float _pitch, const float _vel)
{
  float time, timeKT, releaseVel;
  // env a
  timeKT = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_A_Time_KT) * _pitch;
  releaseVel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_A_Rel_Vel) * _vel;
  m_env_a.m_timeFactor[_voiceId][3] = m_convert->eval_level(timeKT + releaseVel) * m_millisecond;
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_A_Rel);
  if(time <= env_highest_finite_time)
  {
    time *= m_env_a.m_timeFactor[_voiceId][3];
    m_env_a.setSegmentDx(_voiceId, 4, 1.0f / (time + 1.0f));
  }
  else
  {
    m_env_a.setSegmentDx(_voiceId, 4, 0.0f);
  }
  // env b
  timeKT = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_B_Time_KT) * _pitch;
  releaseVel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_B_Rel_Vel) * _vel;
  m_env_b.m_timeFactor[_voiceId][3] = m_convert->eval_level(timeKT + releaseVel) * m_millisecond;
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_B_Rel);
  if(time <= env_highest_finite_time)
  {
    time *= m_env_b.m_timeFactor[_voiceId][3];
    m_env_b.setSegmentDx(_voiceId, 4, 1.0f / (time + 1.0f));
  }
  else
  {
    m_env_b.setSegmentDx(_voiceId, 4, 0.0f);
  }
  // env c
  timeKT = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_C_Time_KT) * _pitch;
  releaseVel = -m_smoothers.get(C15::Smoothers::Poly_Sync::Env_C_Rel_Vel) * _vel;
  m_env_c.m_timeFactor[_voiceId][3] = m_convert->eval_level(timeKT + releaseVel) * m_millisecond;
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_C_Rel);
  if(time <= env_highest_finite_time)
  {
    time *= m_env_c.m_timeFactor[_voiceId][3];
    m_env_c.setSegmentDx(_voiceId, 4, 1.0f / (time + 1.0f));
  }
  else
  {
    m_env_c.setSegmentDx(_voiceId, 4, 0.0f);
  }
  // stop envelopes
  m_env_a.stop(_voiceId);
  m_env_b.stop(_voiceId);
  m_env_c.stop(_voiceId);
  m_env_g.stop(_voiceId);
}

void PolySection::updateEnvLevels(const uint32_t _voiceId)
{
  float peak, dest;
  // env a
  peak = m_env_a.m_levelFactor[_voiceId];
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_A_BP);
  m_env_a.setSegmentDest(_voiceId, 2, true, dest);
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_A_Sus);
  m_env_a.setSegmentDest(_voiceId, 3, true, dest);
  // env b
  peak = m_env_b.m_levelFactor[_voiceId];
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_B_BP);
  m_env_b.setSegmentDest(_voiceId, 2, true, dest);
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_B_Sus);
  m_env_b.setSegmentDest(_voiceId, 3, true, dest);
  // env c
  peak = m_env_c.m_levelFactor[_voiceId];
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_C_BP);
  m_env_c.setSegmentDest(_voiceId, 2, dest);
  dest = peak * m_smoothers.get(C15::Smoothers::Poly_Fast::Env_C_Sus);
  m_env_c.setSegmentDest(_voiceId, 3, dest);
}

void PolySection::updateEnvTimes(const uint32_t _voiceId)
{
  float time;
  // env a
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_A_Att) * m_env_a.m_timeFactor[_voiceId][0];
  m_env_a.setSegmentDx(_voiceId, 1, 1.0f / (time + 1.0f));
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_A_Dec_1) * m_env_a.m_timeFactor[_voiceId][1];
  m_env_a.setSegmentDx(_voiceId, 2, 1.0f / (time + 1.0f));
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_A_Dec_2) * m_env_a.m_timeFactor[_voiceId][2];
  m_env_a.setSegmentDx(_voiceId, 3, 1.0f / (time + 1.0f));
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_A_Rel);
  if(time <= env_highest_finite_time)
  {
    time *= m_env_a.m_timeFactor[_voiceId][3];
    m_env_a.setSegmentDx(_voiceId, 4, 1.0f / (time + 1.0f));
  }
  else
  {
    m_env_a.setSegmentDx(_voiceId, 4, 0.0f);
  }
  // env b
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_B_Att) * m_env_b.m_timeFactor[_voiceId][0];
  m_env_b.setSegmentDx(_voiceId, 1, 1.0f / (time + 1.0f));
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_B_Dec_1) * m_env_b.m_timeFactor[_voiceId][1];
  m_env_b.setSegmentDx(_voiceId, 2, 1.0f / (time + 1.0f));
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_B_Dec_2) * m_env_b.m_timeFactor[_voiceId][2];
  m_env_b.setSegmentDx(_voiceId, 3, 1.0f / (time + 1.0f));
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_B_Rel);
  if(time <= env_highest_finite_time)
  {
    time *= m_env_b.m_timeFactor[_voiceId][3];
    m_env_b.setSegmentDx(_voiceId, 4, 1.0f / (time + 1.0f));
  }
  else
  {
    m_env_b.setSegmentDx(_voiceId, 4, 0.0f);
  }
  // env c
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_C_Att) * m_env_c.m_timeFactor[_voiceId][0];
  m_env_c.setSegmentDx(_voiceId, 1, 1.0f / (time + 1.0f));
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_C_Dec_1) * m_env_c.m_timeFactor[_voiceId][1];
  m_env_c.setSegmentDx(_voiceId, 2, 1.0f / (time + 1.0f));
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_C_Dec_2) * m_env_c.m_timeFactor[_voiceId][2];
  m_env_c.setSegmentDx(_voiceId, 3, 1.0f / (time + 1.0f));
  time = m_smoothers.get(C15::Smoothers::Poly_Slow::Env_C_Rel);
  if(time <= env_highest_finite_time)
  {
    time *= m_env_c.m_timeFactor[_voiceId][3];
    m_env_c.setSegmentDx(_voiceId, 4, 1.0f / (time + 1.0f));
  }
  else
  {
    m_env_c.setSegmentDx(_voiceId, 4, 0.0f);
  }
}
