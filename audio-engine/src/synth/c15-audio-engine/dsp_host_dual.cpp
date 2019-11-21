#include "dsp_host_dual.h"

/******************************************************************************/
/** @file       dsp_host_dual.cpp
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new main engine container
    @todo
*******************************************************************************/

#include <nltools/logging/Log.h>

dsp_host_dual::dsp_host_dual()
{
  m_mainOut_L = m_mainOut_R = 0.0f;
  m_layer_mode = m_preloaded_layer_mode = C15::Properties::LayerMode::Single;
}

void dsp_host_dual::init(const uint32_t _samplerate, const uint32_t _polyphony)
{
  const float samplerate = static_cast<float>(_samplerate);
  // init of crucial components: voiceAlloc, conversion, clock, time, ae_fade_table ("fadepoint"), ae_fader ("pickup")
  m_alloc.init(&m_layer_mode, &m_preloaded_layer_mode);
  m_convert.init();
  m_clock.init(_samplerate);
  m_time.init(_samplerate);
  m_fade.init(samplerate);
  m_output_mute.init(&m_fade.m_value);
  // these could also be part of parameter list (later), currently hard-coded
  m_edit_time.init(C15::Properties::SmootherScale::Linear, 200.0f, 0.0f, 0.1f);
  m_transition_time.init(C15::Properties::SmootherScale::Expon_Env_Time, 1.0f, -20.0f, 0.0f);
  m_reference.init(C15::Properties::SmootherScale::Linear, 80.0f, 400.0f, 0.5f);
  m_reference.m_scaled = scale(m_reference.m_scaling, m_reference.m_position);
  // dsp sections init
  m_global.init(m_time.m_sample_inc);
  m_global.update_tone_amplitude(-12.0f);
  m_global.update_tone_frequency(m_reference.m_scaled);
  m_global.update_tone_mode(0);
  // init poly: exponentiator, feedback pointers
  m_poly[0].init(&m_convert, &m_poly_feedback[2], &m_poly_feedback[3], &m_mono[0].m_dry, &m_mono[0].m_wet,
                 &m_mono[1].m_dry, &m_mono[1].m_wet, &m_reference.m_scaled, m_time.m_millisecond, env_init_gateRelease,
                 samplerate);
  m_poly[1].init(&m_convert, &m_poly_feedback[0], &m_poly_feedback[1], &m_mono[0].m_dry, &m_mono[0].m_wet,
                 &m_mono[1].m_dry, &m_mono[1].m_wet, &m_reference.m_scaled, m_time.m_millisecond, env_init_gateRelease,
                 samplerate);
  // init parameters by parameter list
  m_params.init_modMatrix();
  for(uint32_t i = 0; i < C15::Config::tcd_elements; i++)
  {
    auto element = C15::ParameterList[i];
    switch(element.m_param.m_type)
    {
      // global parameters need their properties and can directly feed to global signals
      case C15::Descriptors::ParameterType::Global_Parameter:
        m_params.init_global(element);
        switch(element.m_ae.m_signal.m_signal)
        {
          case C15::Descriptors::ParameterSignal::Global_Signal:
            switch(element.m_ae.m_smoother.m_clock)
            {
              case C15::Descriptors::SmootherClock::Audio:
                m_global.add_copy_audio_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
              case C15::Descriptors::SmootherClock::Fast:
                m_global.add_copy_fast_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
              case C15::Descriptors::SmootherClock::Slow:
                m_global.add_copy_slow_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
            }
            break;
        }
        break;
      // macro controls need to know their position in mod matrix (which hw amounts are relevant)
      case C15::Descriptors::ParameterType::Macro_Control:
        m_params.init_macro(element);
        break;
      // (local) modulateable parameters need their properties and can directly feed to either quasipoly or mono signals
      case C15::Descriptors::ParameterType::Modulateable_Parameter:
        m_params.init_modulateable(element);
        switch(element.m_ae.m_signal.m_signal)
        {
          case C15::Descriptors::ParameterSignal::Quasipoly_Signal:
            switch(element.m_ae.m_smoother.m_clock)
            {
              case C15::Descriptors::SmootherClock::Audio:
                m_poly[0].add_copy_audio_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_poly[1].add_copy_audio_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
              case C15::Descriptors::SmootherClock::Fast:
                m_poly[0].add_copy_fast_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_poly[1].add_copy_fast_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
              case C15::Descriptors::SmootherClock::Slow:
                m_poly[0].add_copy_slow_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_poly[1].add_copy_slow_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
            }
            break;
          case C15::Descriptors::ParameterSignal::Mono_Signal:
            switch(element.m_ae.m_smoother.m_clock)
            {
              case C15::Descriptors::SmootherClock::Audio:
                m_mono[0].add_copy_audio_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_mono[1].add_copy_audio_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
              case C15::Descriptors::SmootherClock::Fast:
                m_mono[0].add_copy_fast_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_mono[1].add_copy_fast_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
              case C15::Descriptors::SmootherClock::Slow:
                m_mono[0].add_copy_slow_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_mono[1].add_copy_slow_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
            }
            break;
        }
        break;
      // (local) unmodulateable parameters need their properties and can directly feed to either quasipoly or mono signals
      case C15::Descriptors::ParameterType::Unmodulateable_Parameter:
        m_params.init_unmodulateable(element);
        switch(element.m_ae.m_signal.m_signal)
        {
          case C15::Descriptors::ParameterSignal::Quasipoly_Signal:
            switch(element.m_ae.m_smoother.m_clock)
            {
              case C15::Descriptors::SmootherClock::Audio:
                m_poly[0].add_copy_audio_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_poly[1].add_copy_audio_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
              case C15::Descriptors::SmootherClock::Fast:
                m_poly[0].add_copy_fast_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_poly[1].add_copy_fast_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
              case C15::Descriptors::SmootherClock::Slow:
                m_poly[0].add_copy_slow_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_poly[1].add_copy_slow_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
            }
            break;
          case C15::Descriptors::ParameterSignal::Mono_Signal:
            switch(element.m_ae.m_smoother.m_clock)
            {
              case C15::Descriptors::SmootherClock::Audio:
                m_mono[0].add_copy_audio_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_mono[1].add_copy_audio_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
              case C15::Descriptors::SmootherClock::Fast:
                m_mono[0].add_copy_fast_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_mono[1].add_copy_fast_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
              case C15::Descriptors::SmootherClock::Slow:
                m_mono[0].add_copy_slow_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                m_mono[1].add_copy_slow_id(element.m_ae.m_smoother.m_index, element.m_ae.m_signal.m_index);
                break;
            }
            break;
        }
        break;
      case C15::Descriptors::ParameterType::Macro_Time:
        m_params.init_macro_time(element);
        break;
    }
  }
#if LOG_INIT
  nltools::Log::info("dsp_host_dual::init - engine dsp status: global");
  nltools::Log::info("todo: engine - modulation matrix (triggers, behavior, etc.)");
  nltools::Log::info("todo: engine - dsp components implementation");
  nltools::Log::info("missing: nltools::msg - reference, initial:", m_reference.m_scaled);
  nltools::Log::info("issue: nltools::presetMsg - parameter structure, param ids, unsorted param groups, global group");
  nltools::Log::info("(param_ids: mono_grp, master, voice_grp?, split_point?)");
  nltools::Log::info("todo: initialization??? (transition times, etc.)");
#endif
}

C15::ParameterDescriptor dsp_host_dual::getParameter(const int _id)
{
  if((_id > -1) && (_id < C15::Config::tcd_elements))
  {
#if LOG_DISPATCH
    if(C15::ParameterList[_id].m_param.m_type != C15::Descriptors::ParameterType::None)
    {
      nltools::Log::info("dispatch(", _id, "):", C15::ParameterList[_id].m_pg.m_group_id, "-",
                         C15::ParameterList[_id].m_pg.m_param_label_short);
    }
    else
    {
      nltools::Log::warning("dispatch(", _id, "): None!");
    }
#endif
    return C15::ParameterList[_id];
  }
#if LOG_FAIL
  nltools::Log::warning("dispatch(", _id, "):", "failed!");
#endif
  return m_invalid_param;
}

void dsp_host_dual::logStatus()
{
  nltools::Log::info("engine status:");
  nltools::Log::info("-clock(index:", m_clock.m_index, ", fast:", m_clock.m_fast, ", slow:", m_clock.m_slow, ")");
  nltools::Log::info("-output(left:", m_mainOut_L, ", right:", m_mainOut_R, ", mute:", m_output_mute.get_value(), ")");
  nltools::Log::info("-dsp(dx:", m_time.m_sample_inc, ", ms:", m_time.m_millisecond, ")");
  // maybe, log parameter values by groups?
}

void dsp_host_dual::onMidiMessage(const uint32_t _status, const uint32_t _data0, const uint32_t _data1)
{

#if LOG_MIDI
  nltools::Log::info("midiMsg(status:", _status, ", data0:", _data0, ", data1:", _data1, ")");
#endif
  const uint32_t ch = _status & 15, st = (_status & 127) >> 4;
  uint32_t arg = 0;
  // LPC MIDI Protocol 1.7 transmits every LPC Message as MIDI PitchBend Message! (avoiding TCD Protocol collisions)
  if(st == 6)
  {
    switch(ch)
    {
      case 0:
        // Pedal 1
        arg = _data1 + (_data0 << 7);
        updateHW(0, arg);
        // ...
        break;
      case 1:
        // Pedal 2
        arg = _data1 + (_data0 << 7);
        updateHW(1, arg);
        // ...
        break;
      case 2:
        // Pedal 3
        arg = _data1 + (_data0 << 7);
        updateHW(2, arg);
        // ...
        break;
      case 3:
        // Pedal 4
        arg = _data1 + (_data0 << 7);
        updateHW(3, arg);
        // ...
        break;
      case 4:
        // Bender
        arg = _data1 + (_data0 << 7);
        updateHW(4, arg);
        // ...
        break;
      case 5:
        // Aftertouch
        arg = _data1 + (_data0 << 7);
        updateHW(5, arg);
        // ...
        break;
      case 6:
        // Ribbon 1
        arg = _data1 + (_data0 << 7);
        updateHW(6, arg);
        // ...
        break;
      case 7:
        // Ribbon 2
        arg = _data1 + (_data0 << 7);
        updateHW(7, arg);
        // ...
        break;
      case 13:
        // Key Pos (down or up)
        arg = _data1;
        m_key_pos = arg - C15::Config::key_from;
        // key position is only valid within range [36 ... 96]
        m_key_valid = (arg >= C15::Config::key_from) && (arg <= C15::Config::key_to);
        break;
      case 14:
        // Key Down
        arg = _data1 + (_data0 << 7);
        if(m_key_valid)
        {
          keyDown(static_cast<float>(arg) * m_norm_vel);
        }
#if LOG_FAIL
        else
        {
          nltools::Log::warning("key_down(pos:", m_key_pos, ") failed!");
        }
#endif
        // ...
        break;
      case 15:
        // Key Up
        arg = _data1 + (_data0 << 7);
        if(m_key_valid)
        {
          keyUp(static_cast<float>(arg) * m_norm_vel);
        }
#if LOG_FAIL
        else
        {
          nltools::Log::warning("key_up(pos:", m_key_pos, ") failed!");
        }
#endif
        break;
      default:
        break;
    }
  }
}

void dsp_host_dual::onRawMidiMessage(const uint32_t _status, const uint32_t _data0, const uint32_t _data1)
{
  const uint32_t type = (_status & 127) >> 4;
  uint32_t arg = 0;
  switch(type)
  {
    case 0:
      // note off
      arg = static_cast<uint32_t>(static_cast<float>(_data1) * m_format_vel);
      onMidiMessage(0xED, 0, _data0);            // keyPos
      onMidiMessage(0xEF, arg >> 7, arg & 127);  // keyUp
      break;
    case 1:
      // note on
      arg = static_cast<uint32_t>(static_cast<float>(_data1) * m_format_vel);
      onMidiMessage(0xED, 0, _data0);  // keyPos
      if(arg != 0)
      {
        onMidiMessage(0xEE, arg >> 7, arg & 127);  // keyDown
      }
      else
      {
        onMidiMessage(0xEF, 0, 0);  // keyUp
      }
      break;
    case 3:
      // ctrl chg (hw sources: pedals, ribbons) - hard coded ReMOTE template "MSE 2"
      arg = static_cast<uint32_t>(static_cast<float>(_data1) * m_format_hw);
      switch(_data0)
      {
        case 61:
          // Pedal 1
          onMidiMessage(0xE0, arg >> 7, arg & 127);
          break;
        case 62:
          // Pedal 2
          onMidiMessage(0xE1, arg >> 7, arg & 127);
          break;
        case 63:
          // Pedal 3
          onMidiMessage(0xE2, arg >> 7, arg & 127);
          break;
        case 64:
          // Pedal 4
          onMidiMessage(0xE3, arg >> 7, arg & 127);
          break;
        case 67:
          // Ribbon 1
          onMidiMessage(0xE6, arg >> 7, arg & 127);
          break;
        case 68:
          // Ribbon 2
          onMidiMessage(0xE7, arg >> 7, arg & 127);
          break;
        default:
          break;
      }
      break;
    case 5:
      // mono at (hw source)
      arg = static_cast<uint32_t>(static_cast<float>(_data0) * m_format_hw);
      onMidiMessage(0xE5, arg >> 7, arg & 127);  // hw_at
      break;
    case 6:
      // bend (hw source)
      arg = static_cast<uint32_t>(static_cast<float>(_data0 + (_data1 << 7)) * m_format_pb);
      onMidiMessage(0xE4, arg >> 7, arg & 127);  // hw_bend
      break;
    default:
      break;
  }
}

void dsp_host_dual::onPresetMessage(const nltools::msg::SinglePresetMessage &_msg)
{
  m_layer_changed = m_layer_mode != C15::Properties::LayerMode::Single;
  m_preloaded_layer_mode = C15::Properties::LayerMode::Single;
  m_preloaded_single_data = _msg;
#if LOG_RECALL
  // log preset with primitive timestamp (for debugging fade events)
  nltools::Log::info("Received Single Preset Message! (@", m_clock.m_index, ")");
#endif
  if(m_glitch_suppression)
  {
    // glitch suppression: start outputMute fade
    m_fade.enable(FadeEvent::RecallMute, 0);  // enable fader with event
    m_output_mute.pick(0);                    // pickup fade-out
  }
  else
  {
    // direct apply: recall single preset buffer
    m_layer_mode = m_preloaded_layer_mode;
    recallSingle();
  }
}

void dsp_host_dual::onPresetMessage(const nltools::msg::SplitPresetMessage &_msg)
{
  m_layer_changed = m_layer_mode != C15::Properties::LayerMode::Split;
  m_preloaded_layer_mode = C15::Properties::LayerMode::Split;
  m_preloaded_split_data = _msg;
#if LOG_RECALL
  nltools::Log::info("Received Split Preset Message!, (@", m_clock.m_index, ")");
#endif
  if(m_glitch_suppression)
  {
    m_fade.enable(FadeEvent::RecallMute, 0);
    m_output_mute.pick(0);
  }
  else
  {
    m_layer_mode = m_preloaded_layer_mode;
    recallSplit();
  }
}

void dsp_host_dual::onPresetMessage(const nltools::msg::LayerPresetMessage &_msg)
{
  m_layer_changed = m_layer_mode != C15::Properties::LayerMode::Layer;
  m_preloaded_layer_mode = C15::Properties::LayerMode::Layer;
  m_preloaded_layer_data = _msg;
#if LOG_RECALL
  nltools::Log::info("Received Layer Preset Message!, (@", m_clock.m_index, ")");
#endif
  if(m_glitch_suppression)
  {
    m_fade.enable(FadeEvent::RecallMute, 0);
    m_output_mute.pick(0);
  }
  else
  {
    m_layer_mode = m_preloaded_layer_mode;
    recallLayer();
  }
}

void dsp_host_dual::globalParChg(const uint32_t _id, const nltools::msg::UnmodulateableParameterChangedMessage &_msg)
{
  auto param = m_params.get_global(_id);
  if(param->update_position(static_cast<float>(_msg.controlPosition)))
  {
    param->m_scaled = scale(param->m_scaling, param->m_position);
#if LOG_EDITS
    nltools::Log::info("global_edit(pos:", param->m_position, ", val:", param->m_scaled, ")");
#endif
    globalTransition(param, m_edit_time.m_dx);
  }
}
void dsp_host_dual::globalParChg(const uint32_t _id, const nltools::msg::HWSourceChangedMessage &_msg)
{
  auto param = m_params.get_hw_src(_id);
  if(param->update_behavior(getBehavior(_msg.returnMode)))
  {
    param->update_position(static_cast<float>(_msg.controlPosition));
#if LOG_EDITS
    nltools::Log::info("hw_edit(pos:", param->m_position, ", behavior:", static_cast<int>(param->m_behavior), ")");
#endif
#if LOG_MISSING
    nltools::Log::info("todo: trigger hw behavior changed ...");
#endif
  }
  else if(param->update_position(static_cast<float>(_msg.controlPosition)))
  {
#if LOG_EDITS
    nltools::Log::info("hw_edit(pos:", param->m_position, ")");
#endif
#if LOG_MISSING
    nltools::Log::info("todo: trigger hw mod_chain ...");
#endif
  }
}
void dsp_host_dual::localParChg(const uint32_t _id, const nltools::msg::HWAmountChangedMessage &_msg)
{
  const uint32_t layerId = getLayerId(_msg.voiceGroup);
  auto param = m_params.get_hw_amt(layerId, _id);
  if(param->update_position(static_cast<float>(_msg.controlPosition)))
  {
#if LOG_EDITS
    nltools::Log::info("ha_edit(layer:", layerId, ", pos:", param->m_position, ")");
#endif
#if LOG_MISSING
    nltools::Log::info("todo: trigger (silent) mc offset update ...");
#endif
  }
}
void dsp_host_dual::localParChg(const uint32_t _id, const nltools::msg::MacroControlChangedMessage &_msg)
{
  const uint32_t layerId = getLayerId(_msg.voiceGroup);
  auto param = m_params.get_macro(layerId, _id);
#if LOG_MISSING
  nltools::Log::info("mc_edit: missing aspect/trigger for mod_reset");
#endif
  if(param->update_position(static_cast<float>(_msg.controlPosition)))
  {
    param->update_modulation_aspects();  // re-evaluate relative offset to hw_modulation after editing control pos
#if LOG_EDITS
    nltools::Log::info("mc_edit(layer:", layerId, ", pos:", param->m_position, ")");
#endif
    if(m_layer_mode == LayerMode::Single)
    {
      updateModChain(param);
    }
    else
    {
      updateModChain(layerId, param);
    }
  }
}
void dsp_host_dual::localParChg(const uint32_t _id, const nltools::msg::UnmodulateableParameterChangedMessage &_msg)
{
  const uint32_t layerId = getLayerId(_msg.voiceGroup);
  auto param = m_params.get_direct(layerId, _id);
  if(param->update_position(static_cast<float>(_msg.controlPosition)))
  {
    param->m_scaled = scale(param->m_scaling, param->m_position);
#if LOG_EDITS
    nltools::Log::info("direct_edit(layer:", layerId, ", pos:", param->m_position, ", val:", param->m_scaled, ")");
#endif
    if(m_layer_mode == LayerMode::Single)
    {
      localTransition(0, param, m_edit_time.m_dx);
      localTransition(1, param, m_edit_time.m_dx);
    }
    else
    {
      localTransition(layerId, param, m_edit_time.m_dx);
    }
  }
}
void dsp_host_dual::localParChg(const uint32_t _id, const nltools::msg::ModulateableParameterChangedMessage &_msg)
{
  const uint32_t layerId = getLayerId(_msg.voiceGroup), macroId = getMacroId(_msg.sourceMacro);
  auto param = m_params.get_target(layerId, _id);
  bool aspect_update = param->update_source(getMacro(_msg.sourceMacro));
  if(aspect_update)
  {
    m_params.m_layer[layerId].m_assignment.reassign(_id, macroId);
  }
  aspect_update |= param->update_amount(static_cast<float>(_msg.mcAmount));
  if(param->update_position(param->depolarize(static_cast<float>(_msg.controlPosition))))
  {
    param->update_modulation_aspects(m_params.get_macro(layerId, macroId)->m_position);
    param->m_scaled = scale(param->m_scaling, param->polarize(param->m_position));
#if LOG_EDITS
    nltools::Log::info("target_edit(layer:", layerId, ", pos:", param->m_position, ", val:", param->m_scaled, ")");
    if(aspect_update)
    {
      nltools::Log::info("target_edit(layer:", layerId, ", mc:", macroId, ", amt:", param->m_amount, ")");
    }
#endif
    if(m_layer_mode == LayerMode::Single)
    {
      localTransition(0, param, m_edit_time.m_dx);
      localTransition(1, param, m_edit_time.m_dx);
    }
    else
    {
      localTransition(layerId, param, m_edit_time.m_dx);
    }
  }
  else if(aspect_update)
  {
    param->update_modulation_aspects(m_params.get_macro(layerId, macroId)->m_position);
#if LOG_EDITS
    nltools::Log::info("target_edit(layer:", layerId, ", mc:", macroId, ", amt:", param->m_amount, ")");
#endif
  }
  //
}
void dsp_host_dual::localTimeChg(const uint32_t _id, const nltools::msg::UnmodulateableParameterChangedMessage &_msg)
{
  const uint32_t layerId = getLayerId(_msg.voiceGroup);
  auto param = m_params.get_macro_time(layerId, _id);
  if(param->update_position(static_cast<float>(_msg.controlPosition)))
  {
#if LOG_EDITS
    nltools::Log::info("mc_edit(layer:", layerId, ", time:", param->m_position, ")");
#endif
    updateTime(&param->m_dx, scale(param->m_scaling, param->m_position));
  }
}

void dsp_host_dual::localUnisonChg(const nltools::msg::UnmodulateableParameterChangedMessage &_msg)
{
  const uint32_t layerId = getLayerId(_msg.voiceGroup);
  auto param = m_params.get(getLayer(_msg.voiceGroup), C15::Parameters::Unmodulateable_Parameters::Unison_Voices);
  if(param->update_position(static_cast<float>(_msg.controlPosition)))
  {
#if LOG_EDITS
    nltools::Log::info("unison_edit(layer:", layerId, ", pos:", param->m_position, ")");
#endif
    m_alloc.setUnison(layerId, param->m_position);
    const uint32_t uVoice = m_alloc.m_unison - 1;
    if(m_layer_mode == LayerMode::Split)
    {
      m_poly[layerId].resetEnvelopes();
      m_poly[layerId].m_uVoice = uVoice;
      m_poly[layerId].m_key_active = 0;
    }
    else
    {
      m_poly[0].resetEnvelopes();
      m_poly[1].resetEnvelopes();
      m_poly[0].m_uVoice = m_poly[1].m_uVoice = uVoice;
      m_poly[0].m_key_active = m_poly[1].m_key_active = 0;
    }
#if LOG_MISSING
    nltools::Log::info("todo: unison voices should not possess a smoother ...");
#endif
  }
}

void dsp_host_dual::onSettingEditTime(const float _position)
{
  // inconvenient clamping of odd position range ...
  if(m_edit_time.update_position(_position < 0.0f ? 0.0f : _position > 1.0f ? 1.0f : _position))
  {
#if LOG_SETTINGS
    nltools::Log::info("edit_time(pos:", m_edit_time.m_position, ", raw:", _position, ")");
#endif
    updateTime(&m_edit_time.m_dx, scale(m_edit_time.m_scaling, m_edit_time.m_position));
  }
}

void dsp_host_dual::onSettingTransitionTime(const float _position)
{
  // inconvenient clamping of odd position range ...
  if(m_transition_time.update_position(_position < 0.0f ? 0.0f : _position > 1.0f ? 1.0f : _position))
  {
#if LOG_SETTINGS
    nltools::Log::info("transition_time(pos:", m_transition_time.m_position, ", raw:", _position, ")");
#endif
    updateTime(&m_transition_time.m_dx, scale(m_transition_time.m_scaling, m_transition_time.m_position));
  }
}

void dsp_host_dual::onSettingNoteShift(const float _shift)
{
  m_poly[0].m_note_shift = m_poly[1].m_note_shift = _shift;
#if LOG_SETTINGS
  nltools::Log::info("note_shift:", _shift);
#endif
}

void dsp_host_dual::onSettingGlitchSuppr(const bool _enabled)
{
  m_glitch_suppression = _enabled;
#if LOG_SETTINGS
  nltools::Log::info("glitch_suppression:", _enabled);
#endif
}

void dsp_host_dual::onSettingInitialSinglePreset()
{
#if LOG_RECALL
  nltools::Log::info("recallInitialSinglePreset(@", m_clock.m_index, ")");
  auto unison = m_params.get(C15::Properties::LayerId::I, C15::Parameters::Unmodulateable_Parameters::Unison_Voices);
  unison->update_position(unison->m_initial);
#endif
#if LOG_RESET
  nltools::Log::info("recall single voice reset");
#endif
  m_alloc.setUnison(0, unison->m_position);
  const uint32_t uVoice = m_alloc.m_unison - 1;
  m_poly[0].resetEnvelopes();
  m_poly[1].resetEnvelopes();
  m_poly[0].m_uVoice = m_poly[1].m_uVoice = uVoice;
  m_poly[0].m_key_active = m_poly[1].m_key_active = 0;
  m_params.m_layer[0].m_assignment.reset();
  m_params.m_layer[1].m_assignment.reset();
  for(uint32_t i = 0; i < m_params.m_layer[0].m_macro_count; i++)
  {
    m_params.m_layer[0].m_macro[i].update_position(0.5f);
    m_params.m_layer[0].m_macro[i].update_modulation_aspects();
    m_params.m_layer[1].m_macro[i].update_position(0.5f);
    m_params.m_layer[1].m_macro[i].update_modulation_aspects();
  }
#if LOG_RECALL
  nltools::Log::info("recall: hw_sources:");
#endif
  for(uint32_t i = 0; i < m_params.m_global.m_source_count; i++)
  {
    auto param = m_params.get_hw_src(i);
    //param->update_behavior(C15::Properties::HW_Return_Behavior::Zero);
    param->update_position(param->m_initial);
  }
#if LOG_RECALL
  nltools::Log::info("recall: global:");
#endif
  for(uint32_t i = 0; i < m_params.m_global.m_direct_count; i++)
  {
    auto param = m_params.get_global(i);
    param->update_position(param->m_initial);
    param->m_scaled = scale(param->m_scaling, param->m_position);
    globalTransition(param, m_transition_time.m_dx);
  }
#if LOG_RECALL
  nltools::Log::info("recall: unmodulateables:");
#endif
  for(uint32_t i = 0; i < m_params.m_layer[0].m_direct_count; i++)
  {
    auto param = m_params.get_direct(0, i);
    param->update_position(param->m_initial);
    param->m_scaled = scale(param->m_scaling, param->m_position);
    localTransition(0, param, m_transition_time.m_dx);
    localTransition(1, param, m_transition_time.m_dx);
  }
#if LOG_MISSING
  nltools::Log::info("todo: re-evaluate hw matrix for macro offsets!");
#endif
#if LOG_RECALL
  nltools::Log::info("recall: macros:");
#endif
  for(uint32_t i = 0; i < m_params.m_layer[0].m_macro_count; i++)
  {
    auto param = m_params.get_macro(0, i);
    param->update_position(param->m_initial);
    param->update_modulation_aspects();
  }
#if LOG_RECALL
  nltools::Log::info("recall: modulateables:");
#endif
  for(uint32_t i = 0; i < m_params.m_layer[0].m_target_count; i++)
  {
    auto param = m_params.get_target(0, i);
    param->update_source(C15::Parameters::Macro_Controls::None);
    param->update_amount(0.0f);
    param->update_position(param->depolarize(param->m_initial));
    param->update_modulation_aspects(m_params.get_macro(0, static_cast<uint32_t>(param->m_source))->m_position);
    param->m_scaled = scale(param->m_scaling, param->polarize(param->m_position));
    localTransition(0, param, m_transition_time.m_dx);
    localTransition(1, param, m_transition_time.m_dx);
  }
}

uint32_t dsp_host_dual::onSettingToneToggle()
{
  m_tone_state = (m_tone_state + 1) % 3;
  m_fade.enable(FadeEvent::ToneMute, 0);
  m_output_mute.pick(0);
  return m_tone_state;
}

void dsp_host_dual::render()
{
  // clock & fadepoint rendering
  m_clock.render();
  evalFadePoint();
  // slow rendering
  if(m_clock.m_slow)
  {
    // render components
    m_global.render_slow();
    m_poly[0].render_slow(m_global.m_signals.get(C15::Signals::Global_Signals::Master_Tune));
    m_poly[1].render_slow(m_global.m_signals.get(C15::Signals::Global_Signals::Master_Tune));
    m_mono[0].render_slow();
    m_mono[1].render_slow();
  }
  // fast rendering
  if(m_clock.m_fast)
  {
    // render components
    m_global.render_fast();
    m_poly[0].render_fast();
    m_poly[1].render_fast();
    m_mono[0].render_fast();
    m_mono[1].render_fast();
  }
  // audio rendering (always)
  const float mute = m_output_mute.get_value();
  // - resolve poly feedback
  m_poly_feedback[0] = m_poly[0].m_osc_a;
  m_poly_feedback[1] = m_poly[0].m_osc_b;
  m_poly_feedback[2] = m_poly[1].m_osc_a;
  m_poly_feedback[3] = m_poly[1].m_osc_b;
  // - audio dsp poly - both layers
  m_poly[0].render_audio(mute);
  m_poly[1].render_audio(mute);
  // - audio dsp mono - each layer with separate sends - left, right)
  m_mono[0].render_audio(m_poly[0].m_send0_l + m_poly[1].m_send0_l, m_poly[0].m_send0_r + m_poly[1].m_send0_r);
  m_mono[1].render_audio(m_poly[0].m_send1_l + m_poly[1].m_send1_l, m_poly[0].m_send1_r + m_poly[1].m_send1_r);
  // - audio dsp global - main out: combine layers, apply test_tone and soft clip
  m_global.render_audio(m_mono[0].m_out_l, m_mono[0].m_out_r, m_mono[1].m_out_l, m_mono[1].m_out_r);
  // - final: main out, output mute
  m_mainOut_L = m_global.m_out_l * mute;
  m_mainOut_R = m_global.m_out_r * mute;
}

void dsp_host_dual::reset()
{
#if LOG_RESET
  nltools::Log::info("dsp_reset()");
#endif
}

C15::Properties::HW_Return_Behavior dsp_host_dual::getBehavior(const ReturnMode _mode)
{
  switch(_mode)
  {
    case ReturnMode::None:
      return C15::Properties::HW_Return_Behavior::Stay;
    case ReturnMode::Zero:
      return C15::Properties::HW_Return_Behavior::Zero;
    case ReturnMode::Center:
      return C15::Properties::HW_Return_Behavior::Center;
    default:
      return C15::Properties::HW_Return_Behavior::Stay;
  }
}

C15::Properties::HW_Return_Behavior dsp_host_dual::getBehavior(const RibbonReturnMode _mode)
{
  switch(_mode)
  {
    case RibbonReturnMode::STAY:
      return C15::Properties::HW_Return_Behavior::Stay;
    case RibbonReturnMode::RETURN:
      return C15::Properties::HW_Return_Behavior::Center;
    default:
      return C15::Properties::HW_Return_Behavior::Stay;
  }
}

C15::Parameters::Macro_Controls dsp_host_dual::getMacro(const MacroControls _mc)
{
  switch(_mc)
  {
    case MacroControls::NONE:
      return C15::Parameters::Macro_Controls::None;
    case MacroControls::MC1:
      return C15::Parameters::Macro_Controls::MC_A;
    case MacroControls::MC2:
      return C15::Parameters::Macro_Controls::MC_B;
    case MacroControls::MC3:
      return C15::Parameters::Macro_Controls::MC_C;
    case MacroControls::MC4:
      return C15::Parameters::Macro_Controls::MC_D;
#if 0
    case MacroControls::MC5:
      return C15::Parameters::Macro_Controls::MC_E;
    case MacroControls::MC6:
      return C15::Parameters::Macro_Controls::MC_F;
#endif
    default:
      return C15::Parameters::Macro_Controls::None;
  }
  // maybe, a simple static_cast would be sufficient ...
}

uint32_t dsp_host_dual::getMacroId(const MacroControls _mc)
{
  return static_cast<uint32_t>(_mc);  // assuming idential MC enumeration
}

C15::Properties::LayerId dsp_host_dual::getLayer(const VoiceGroup _vg)
{
  switch(_vg)
  {
    case VoiceGroup::I:
      return C15::Properties::LayerId::I;
    case VoiceGroup::II:
      return C15::Properties::LayerId::II;
    default:
      return C15::Properties::LayerId::I;
  }
}

uint32_t dsp_host_dual::getLayerId(const VoiceGroup _vg)
{
  switch(_vg)
  {
    case VoiceGroup::I:
      return 0;
    case VoiceGroup::II:
      return 1;
    default:
      return 0;
  }
}

void dsp_host_dual::keyDown(const float _vel)
{
  if(m_alloc.keyDown(m_key_pos))
  {
    // get centered, scaled, note_shifted keyTune from position (C3 -> 0.0 - relative tuning is added smoothed, in post processing)
    const float keyTune = m_global.key_position(m_key_pos);
#if LOG_KEYS
    nltools::Log::info("key_down(tune:", keyTune, ", vel:", _vel, ", unison:", m_alloc.m_unison, ")");
#endif
    for(auto key = m_alloc.m_traversal.first(); m_alloc.m_traversal.running(); key = m_alloc.m_traversal.next())
    {
      m_poly[key->m_localIndex].keyDown(key->m_voiceId, key->m_unisonIndex, key->m_stolen, keyTune, _vel);
#if LOG_KEYS_POLY
      nltools::Log::info("key_down_poly(voice:", key->m_voiceId, ", unisonIndex:", key->m_unisonIndex,
                         ", stolen:", key->m_stolen, ", tune:", keyTune, ", velocity:", _vel, ")");
#endif
    }
    const uint32_t index = m_alloc.m_traversal.first()->m_localIndex;
    if(m_layer_mode == LayerMode::Split)
    {
      // only in split mode, the mono flanger envelope should be started in corresponding mono section
      // flanger legato
      if(m_poly[index].m_key_active == 0)
      {
        m_mono[index].keyDown(_vel);
      }
    }
    else
    {
      // in single and layer mode, both mono flanger envelopes should be started
      // flanger legato
      if(m_poly[0].m_key_active == 0)
      {
        m_mono[0].keyDown(_vel);
      }
      if(m_poly[1].m_key_active == 0)
      {
        m_mono[1].keyDown(_vel);
      }
    }
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("keyDown(pos:", m_key_pos, ") failed!");
  }
#endif
}

void dsp_host_dual::keyUp(const float _vel)
{
  if(m_alloc.keyUp(m_key_pos))
  {
    // get centered, scaled, note_shifted keyTune from position (C3 -> 0.0 - relative tuning is added smoothed, in post processing)
    const float keyTune = m_global.key_position(m_key_pos);
#if LOG_KEYS
    nltools::Log::info("key_up(tune:", keyTune, ", vel:", _vel, ", unison:", m_alloc.m_unison, ")");
#endif
    for(auto key = m_alloc.m_traversal.first(); m_alloc.m_traversal.running(); key = m_alloc.m_traversal.next())
    {
      m_poly[key->m_localIndex].keyUp(key->m_voiceId, key->m_unisonIndex, keyTune, _vel);
#if LOG_KEYS_POLY
      nltools::Log::info("key_up_poly(voice:", key->m_voiceId, ", tune:", keyTune, ", velocity:", _vel, ")");
#endif
    }
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("keyUp(pos:", m_key_pos, ") failed!");
  }
#endif
}

float dsp_host_dual::scale(const Scale_Aspect _scl, float _value)
{
  float result = 0.0f;
  switch(_scl.m_id)
  {
    case C15::Properties::SmootherScale::None:
      break;
    case C15::Properties::SmootherScale::Linear:
      result = _scl.m_offset + (_scl.m_factor * _value);
      break;
    case C15::Properties::SmootherScale::Parabolic:
      result = _scl.m_offset + (_scl.m_factor * _value * std::abs(_value));
      break;
    case C15::Properties::SmootherScale::Cubic:
      result = _scl.m_offset + (_scl.m_factor * _value * _value * _value);
      break;
    case C15::Properties::SmootherScale::S_Curve:
      _value = (2.0f * (1.0f - _value)) - 1.0f;
      result = _scl.m_offset + (_scl.m_factor * ((_value * _value * _value * -0.25f) + (_value * 0.75f) + 0.5f));
      break;
    case C15::Properties::SmootherScale::Expon_Gain:
      result = m_convert.eval_level(_scl.m_offset + (_scl.m_factor * _value));
      break;
    case C15::Properties::SmootherScale::Expon_Osc_Pitch:
      result = m_convert.eval_osc_pitch(_scl.m_offset + (_scl.m_factor * _value));
      break;
    case C15::Properties::SmootherScale::Expon_Lin_Pitch:
      result = m_convert.eval_lin_pitch(_scl.m_offset + (_scl.m_factor * _value));
      break;
    case C15::Properties::SmootherScale::Expon_Shaper_Drive:
      result = (m_convert.eval_level(_value * _scl.m_factor) * _scl.m_offset) - _scl.m_offset;
      break;
    case C15::Properties::SmootherScale::Expon_Mix_Drive:
      result = _scl.m_offset + (_scl.m_factor * m_convert.eval_level(_value));
      break;
    case C15::Properties::SmootherScale::Expon_Env_Time:
      result = m_convert.eval_time((_value * _scl.m_factor * 104.0781f) + _scl.m_offset);
      break;
  }
  return result;
}

void dsp_host_dual::updateHW(const uint32_t _id, const uint32_t _raw)
{
#if LOG_MISSING
  nltools::Log::info("todo: implement hw triggers!");
#endif
}

void dsp_host_dual::updateTime(Time_Aspect *_param, const float _ms)
{
  m_time.update_ms(_ms);
  _param->m_dx_audio = m_time.m_dx_audio;
  _param->m_dx_fast = m_time.m_dx_fast;
  _param->m_dx_slow = m_time.m_dx_slow;
#if LOG_TIMES
  nltools::Log::info("time_update(ms:", _ms, ", dx:", m_time.m_dx_audio, m_time.m_dx_fast, m_time.m_dx_slow, ")");
#endif
}

void dsp_host_dual::updateModChain(Macro_Param *_mc)
{
  for(auto param = m_params.chainFirst(0, _mc->m_id); m_params.chainRunning(0); param = m_params.chainNext(0))
  {
    if(param->modulate(_mc->m_position))
    {
      const float clipped = param->m_unclipped < 0.0f ? 0.0f : param->m_unclipped > 1.0f ? 1.0f : param->m_unclipped;
      if(param->m_position != clipped)
      {
        param->m_position = clipped;
        param->m_scaled = scale(param->m_scaling, param->polarize(clipped));
        localTransition(0, param, _mc->m_time.m_dx);
        localTransition(1, param, _mc->m_time.m_dx);
      }
    }
  }
}

void dsp_host_dual::updateModChain(const uint32_t _layer, Macro_Param *_mc)
{
  for(auto param = m_params.chainFirst(_layer, _mc->m_id); m_params.chainRunning(_layer);
      param = m_params.chainNext(_layer))
  {
    if(param->modulate(_mc->m_position))
    {
      const float clipped = param->m_unclipped < 0.0f ? 0.0f : param->m_unclipped > 1.0f ? 1.0f : param->m_unclipped;
      if(param->m_position != clipped)
      {
        param->m_position = clipped;
        param->m_scaled = scale(param->m_scaling, param->polarize(clipped));
        localTransition(_layer, param, _mc->m_time.m_dx);
      }
    }
  }
}

void dsp_host_dual::globalTransition(const Direct_Param *_param, const Time_Aspect _time)
{
  float dx = 0.0f, dest = _param->m_scaled;
  bool valid = true;
  switch(_param->m_render.m_section)
  {
    case C15::Descriptors::SmootherSection::Global:
      switch(_param->m_render.m_clock)
      {
        case C15::Descriptors::SmootherClock::Sync:
          m_global.start_sync(_param->m_render.m_index, dest);
          break;
        case C15::Descriptors::SmootherClock::Audio:
          dx = _time.m_dx_audio;
          m_global.start_audio(_param->m_render.m_index, dx, dest);
          break;
        case C15::Descriptors::SmootherClock::Fast:
          dx = _time.m_dx_fast;
          m_global.start_fast(_param->m_render.m_index, dx, dest);
          break;
        case C15::Descriptors::SmootherClock::Slow:
          dx = _time.m_dx_slow;
          m_global.start_slow(_param->m_render.m_index, dx, dest);
          break;
      }
      break;
    default:
      valid = false;
      break;
  }
#if LOG_TRANSITIONS
  if(valid)
  {
    nltools::Log::info("global_transition(index:", _param->m_render.m_index, ", dx:", dx, ", dest:", dest, ")");
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("failed to start global_transition(index:", _param->m_render.m_index, ")");
  }
#endif
#endif
}
void dsp_host_dual::localTransition(const uint32_t _layer, const Direct_Param *_param, const Time_Aspect _time)
{
  float dx = 0.0f, dest = _param->m_scaled;
  bool valid = true;
  switch(_param->m_render.m_section)
  {
    case C15::Descriptors::SmootherSection::Poly:
      switch(_param->m_render.m_clock)
      {
        case C15::Descriptors::SmootherClock::Sync:
          m_poly[_layer].start_sync(_param->m_render.m_index, dest);
          break;
        case C15::Descriptors::SmootherClock::Audio:
          dx = _time.m_dx_audio;
          m_poly[_layer].start_audio(_param->m_render.m_index, dx, dest);
          break;
        case C15::Descriptors::SmootherClock::Fast:
          dx = _time.m_dx_fast;
          m_poly[_layer].start_fast(_param->m_render.m_index, dx, dest);
          break;
        case C15::Descriptors::SmootherClock::Slow:
          dx = _time.m_dx_slow;
          m_poly[_layer].start_slow(_param->m_render.m_index, dx, dest);
          break;
      }
      break;
    case C15::Descriptors::SmootherSection::Mono:
      switch(_param->m_render.m_clock)
      {
        case C15::Descriptors::SmootherClock::Sync:
          m_mono[_layer].start_sync(_param->m_render.m_index, dest);
          break;
        case C15::Descriptors::SmootherClock::Audio:
          dx = _time.m_dx_audio;
          m_mono[_layer].start_audio(_param->m_render.m_index, dx, dest);
          break;
        case C15::Descriptors::SmootherClock::Fast:
          dx = _time.m_dx_fast;
          m_mono[_layer].start_fast(_param->m_render.m_index, dx, dest);
          break;
        case C15::Descriptors::SmootherClock::Slow:
          dx = _time.m_dx_slow;
          m_mono[_layer].start_slow(_param->m_render.m_index, dx, dest);
          break;
      }
      break;
    default:
      valid = false;
      break;
  }
#if LOG_TRANSITIONS
  if(valid)
  {
    nltools::Log::info("local_transition(layer:", _layer, "index:", _param->m_render.m_index, ", dx:", dx,
                       ", dest:", dest, ")");
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("failed to start global_transition(index:", _param->m_render.m_index, ")");
  }
#endif
#endif
}
void dsp_host_dual::localTransition(const uint32_t _layer, const Target_Param *_param, const Time_Aspect _time)
{
  float dx = 0.0f, dest = _param->m_scaled;
  bool valid = true;
  switch(_param->m_render.m_section)
  {
    case C15::Descriptors::SmootherSection::Poly:
      switch(_param->m_render.m_clock)
      {
        case C15::Descriptors::SmootherClock::Sync:
          m_poly[_layer].start_sync(_param->m_render.m_index, dest);
          break;
        case C15::Descriptors::SmootherClock::Audio:
          dx = _time.m_dx_audio;
          m_poly[_layer].start_audio(_param->m_render.m_index, dx, dest);
          break;
        case C15::Descriptors::SmootherClock::Fast:
          dx = _time.m_dx_fast;
          m_poly[_layer].start_fast(_param->m_render.m_index, dx, dest);
          break;
        case C15::Descriptors::SmootherClock::Slow:
          dx = _time.m_dx_slow;
          m_poly[_layer].start_slow(_param->m_render.m_index, dx, dest);
          break;
      }
      break;
    case C15::Descriptors::SmootherSection::Mono:
      switch(_param->m_render.m_clock)
      {
        case C15::Descriptors::SmootherClock::Sync:
          m_mono[_layer].start_sync(_param->m_render.m_index, dest);
          break;
        case C15::Descriptors::SmootherClock::Audio:
          dx = _time.m_dx_audio;
          m_mono[_layer].start_audio(_param->m_render.m_index, dx, dest);
          break;
        case C15::Descriptors::SmootherClock::Fast:
          dx = _time.m_dx_fast;
          m_mono[_layer].start_fast(_param->m_render.m_index, dx, dest);
          break;
        case C15::Descriptors::SmootherClock::Slow:
          dx = _time.m_dx_slow;
          m_mono[_layer].start_slow(_param->m_render.m_index, dx, dest);
          break;
      }
      break;
    default:
      valid = false;
      break;
  }
#if LOG_TRANSITIONS
  if(valid)
  {
    nltools::Log::info("local_transition(layer:", _layer, "index:", _param->m_render.m_index, ", dx:", dx,
                       ", dest:", dest, ")");
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("failed to start global_transition(index:", _param->m_render.m_index, ")");
  }
#endif
#endif
}

void dsp_host_dual::evalFadePoint()
{
  m_fade.render();
  // act when fade process was completed
  if(m_fade.get_state())
  {
    switch(m_fade.m_event)
    {
      case FadeEvent::RecallMute:
        m_layer_mode = m_preloaded_layer_mode;
        // flush, load preset
        switch(m_layer_mode)
        {
          case C15::Properties::LayerMode::Single:
            recallSingle();
            break;
          case C15::Properties::LayerMode::Split:
            recallSplit();
            break;
          case C15::Properties::LayerMode::Layer:
            recallLayer();
            break;
        }
        m_fade.stop();
        m_output_mute.pick(2);
        m_fade.enable(FadeEvent::RecallUnmute, 1);
        break;
      case FadeEvent::RecallUnmute:
        m_fade.stop();
        m_output_mute.stop();
        break;
      case FadeEvent::ToneMute:
        m_global.update_tone_mode(m_tone_state);
        m_fade.stop();
        m_output_mute.pick(2);
        m_fade.enable(FadeEvent::ToneUnmute, 1);
        break;
      case FadeEvent::ToneUnmute:
        m_fade.stop();
        m_output_mute.stop();
        break;
    }
  }
}

Direct_Param *dsp_host_dual::evalVoiceChg(const C15::Properties::LayerId _layerId,
                                          const nltools::msg::ParameterGroups::UnmodulatebaleParameter &_unisonVoices)
{
  auto param = m_params.get(_layerId, C15::Parameters::Unmodulateable_Parameters::Unison_Voices);
  m_layer_changed |= param->update_position(static_cast<float>(_unisonVoices.controlPosition));
  return param;
}

void dsp_host_dual::recallSingle()
{
#if LOG_RECALL
  nltools::Log::info("recallSingle(@", m_clock.m_index, ")");
#endif
  auto msg = &m_preloaded_single_data;
  // reset detection
  auto unison = evalVoiceChg(C15::Properties::LayerId::I, msg->unisonVoices);
  if(m_layer_changed)
  {
#if LOG_RESET
    nltools::Log::info("recall single voice reset");
#endif
    m_alloc.setUnison(0, unison->m_position);
    const uint32_t uVoice = m_alloc.m_unison - 1;
    m_poly[0].resetEnvelopes();
    m_poly[1].resetEnvelopes();
    m_poly[0].m_uVoice = m_poly[1].m_uVoice = uVoice;
    m_poly[0].m_key_active = m_poly[1].m_key_active = 0;
  }
  // reset macro assignments
  m_params.m_layer[0].m_assignment.reset();
  m_params.m_layer[1].m_assignment.reset();
  // global updates: sources, master, scale (master + scale = global)
#if LOG_RECALL
  nltools::Log::info("recall: hw_sources:");
#endif
  for(uint32_t i = 0; i < msg->pedals.size(); i++)
  {
    globalParRcl(msg->pedals[i]);
  }
  for(uint32_t i = 0; i < msg->bender.size(); i++)
  {
    globalParRcl(msg->bender[i]);
  }
  for(uint32_t i = 0; i < msg->aftertouch.size(); i++)
  {
    globalParRcl(msg->aftertouch[i]);
  }
  for(uint32_t i = 0; i < msg->ribbons.size(); i++)
  {
    globalParRcl(msg->ribbons[i]);
  }
#if LOG_RECALL
  nltools::Log::info("recall: global:");
#endif
  for(uint32_t i = 0; i < msg->master.size(); i++)
  {
    globalParRcl(msg->master[i]);
  }
  for(uint32_t i = 0; i < msg->scale.size(); i++)
  {
    globalParRcl(msg->scale[i]);
  }
  // local updates: params[I] -> transitions[I & II]
  // order: unmodulateables (amounts, mc times, direct), macros, mono (direct | target), modulateables
#if LOG_RECALL
  nltools::Log::info("recall: unmodulateables:");
#endif
  for(uint32_t i = 0; i < msg->unmodulateables.size(); i++)
  {
    localParRcl(0, msg->unmodulateables[i]);
  }
#if LOG_MISSING
  nltools::Log::info("todo: re-evaluate hw matrix for macro offsets!");
#endif
#if LOG_RECALL
  nltools::Log::info("recall: macros:");
#endif
  for(uint32_t i = 0; i < msg->macros.size(); i++)
  {
    localParRcl(0, msg->macros[i]);
  }
#if LOG_MISSING
  nltools::Log::info("todo: update macro mod aspects!");
#endif
#if LOG_RECALL
  nltools::Log::info("recall: monos:");
#endif
  for(uint32_t i = 0; i < msg->monos.size(); i++)
  {
    localParRcl(0, msg->monos[i]);
  }
#if LOG_RECALL
  nltools::Log::info("recall: modulateables:");
#endif
  for(uint32_t i = 0; i < msg->modulateables.size(); i++)
  {
    localParRcl(0, msg->modulateables[i]);
  }
  // start global transitions
  for(uint32_t i = 0; i < m_params.m_global.m_direct_count; i++)
  {
    auto param = m_params.get_global(i);
    if(param->m_changed)
    {
      globalTransition(param, m_transition_time.m_dx);
    }
  }
  // use local params in layer I to retrieve positions
  // start local transitions in both layers and update target mod aspects
  for(uint32_t i = 0; i < m_params.m_layer[0].m_direct_count; i++)
  {
    auto param = m_params.get_direct(0, i);
    if(param->m_changed)
    {
      localTransition(0, param, m_transition_time.m_dx);
      localTransition(1, param, m_transition_time.m_dx);
    }
  }
  for(uint32_t i = 0; i < m_params.m_layer[0].m_target_count; i++)
  {
    auto param = m_params.get_target(0, i);
    param->update_modulation_aspects(m_params.get_macro(0, static_cast<uint32_t>(param->m_source))->m_position);
    if(param->m_changed)
    {
      localTransition(0, param, m_transition_time.m_dx);
      localTransition(1, param, m_transition_time.m_dx);
    }
  }
}

void dsp_host_dual::recallSplit()
{
#if LOG_MISSING
  nltools::Log::info("todo: implement recallSplit()!");
#endif
}

void dsp_host_dual::recallLayer()
{
#if LOG_MISSING
  nltools::Log::info("todo: implement recallLayer()!");
#endif
}

void dsp_host_dual::globalParRcl(const nltools::msg::ParameterGroups::PedalParameter &_source)
{
  auto element = getParameter(_source.id);
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Hardware_Source)
  {

    auto param = m_params.get_hw_src(element.m_param.m_index);
    param->update_behavior(getBehavior(_source.returnMode));
    param->update_position(static_cast<float>(_source.controlPosition));
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("failed to recall Pedal(id:", _source.id, ")");
  }
#endif
}

void dsp_host_dual::globalParRcl(const nltools::msg::ParameterGroups::BenderParameter &_source)
{
  auto element = getParameter(_source.id);
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Hardware_Source)
  {

    auto param = m_params.get_hw_src(element.m_param.m_index);
    param->update_position(static_cast<float>(_source.controlPosition));
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("failed to recall Bender(id:", _source.id, ")");
  }
#endif
}

void dsp_host_dual::globalParRcl(const nltools::msg::ParameterGroups::AftertouchParameter &_source)
{
  auto element = getParameter(_source.id);
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Hardware_Source)
  {
    auto param = m_params.get_hw_src(element.m_param.m_index);
    param->update_position(static_cast<float>(_source.controlPosition));
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("failed to recall Aftertouch(id:", _source.id, ")");
  }
#endif
}

void dsp_host_dual::globalParRcl(const nltools::msg::ParameterGroups::RibbonParameter &_source)
{
  auto element = getParameter(_source.id);
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Hardware_Source)
  {
    auto param = m_params.get_hw_src(element.m_param.m_index);
    param->update_behavior(getBehavior(_source.ribbonReturnMode));
    param->update_position(static_cast<float>(_source.controlPosition));
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("failed to recall Ribbon(id:", _source.id, ")");
  }
#endif
}

void dsp_host_dual::globalParRcl(const nltools::msg::ParameterGroups::Parameter &_source)
{
  auto element = getParameter(_source.id);
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Global_Parameter)
  {
    auto param = m_params.get_global(element.m_param.m_index);
    param->m_changed = false;
    if(param->update_position(static_cast<float>(_source.controlPosition)))
    {
      param->m_scaled = scale(param->m_scaling, param->m_position);
      param->m_changed = true;
    }
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("failed to recall Global(id:", _source.id, ")");
  }
#endif
}

void dsp_host_dual::localParRcl(const uint32_t _layer,
                                const nltools::msg::ParameterGroups::UnmodulatebaleParameter &_source)
{
  auto element = getParameter(_source.id);
  const uint32_t id = element.m_param.m_index;
  switch(element.m_param.m_type)
  {
    case C15::Descriptors::ParameterType::Hardware_Amount:
      m_params.get_hw_amt(_layer, id)->update_position(static_cast<float>(_source.controlPosition));
      break;
    case C15::Descriptors::ParameterType::Macro_Time:
      localTimeRcl(_layer, element.m_param.m_index, static_cast<float>(_source.controlPosition));
      break;
    case C15::Descriptors::ParameterType::Unmodulateable_Parameter:
      localDirectRcl(m_params.get_direct(_layer, id), _source);
      break;
    default:
#if LOG_FAIL
      nltools::Log::warning("failed to recall Unmodulateable(id:", _source.id, ")");
#endif
      break;
  }
}

void dsp_host_dual::localParRcl(const uint32_t _layer, const nltools::msg::ParameterGroups::MacroParameter &_source)
{
  auto element = getParameter(_source.id);
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Macro_Control)
  {
    auto param = m_params.get_macro(_layer, element.m_param.m_index);
    param->update_position(static_cast<float>(_source.controlPosition));
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("failed to recall Macro(id:", _source.id, ")");
  }
#endif
}

void dsp_host_dual::localParRcl(const uint32_t _layer, const nltools::msg::ParameterGroups::MonoParameter &_source)
{
  auto element = getParameter(_source.id);
  switch(element.m_param.m_type)
  {
    case C15::Descriptors::ParameterType::Unmodulateable_Parameter:
      localDirectRcl(m_params.get_direct(_layer, element.m_param.m_index), _source);
      break;
    case C15::Descriptors::ParameterType::Modulateable_Parameter:
      localTargetRcl(m_params.get_target(_layer, element.m_param.m_index), _source);
      break;
    default:
#if LOG_FAIL
      nltools::Log::warning("failed to recall Mono(id:", _source.id, ")");
#endif
      break;
  }
}

void dsp_host_dual::localParRcl(const uint32_t _layer,
                                const nltools::msg::ParameterGroups::ModulateableParameter &_source)
{
  auto element = getParameter(_source.id);
  if(element.m_param.m_type == C15::Descriptors::ParameterType::Modulateable_Parameter)
  {
    localTargetRcl(m_params.get_target(_layer, element.m_param.m_index), _source);
  }
#if LOG_FAIL
  else
  {
    nltools::Log::warning("failed to recall Modulateable(id:", _source.id, ")");
  }
#endif
}

void dsp_host_dual::localTimeRcl(const uint32_t _layer, const uint32_t _id, const float _value)
{
  auto param = m_params.get_macro_time(_layer, _id);
  if(param->update_position(_value))
  {
    updateTime(&param->m_dx, scale(param->m_scaling, param->m_position));
  }
}

void dsp_host_dual::localDirectRcl(Direct_Param *_param,
                                   const nltools::msg::ParameterGroups::UnmodulatebaleParameter &_source)
{
  _param->m_changed = false;
  if(_param->update_position(static_cast<float>(_source.controlPosition)))
  {
    _param->m_scaled = scale(_param->m_scaling, _param->m_position);
    _param->m_changed = true;
  }
}

void dsp_host_dual::localDirectRcl(Direct_Param *_param, const nltools::msg::ParameterGroups::MonoParameter &_source)
{
  _param->m_changed = false;
  if(_param->update_position(static_cast<float>(_source.controlPosition)))
  {
    _param->m_scaled = scale(_param->m_scaling, _param->m_position);
    _param->m_changed = true;
  }
}

void dsp_host_dual::localTargetRcl(Target_Param *_param,
                                   const nltools::msg::ParameterGroups::ModulateableParameter &_source)
{
  _param->m_changed = false;
  _param->update_source(getMacro(_source.mc));
  _param->update_amount(static_cast<float>(_source.modulationAmount));
  if(_param->update_position(static_cast<float>(_source.controlPosition)))
  {
    _param->m_scaled = scale(_param->m_scaling, _param->polarize(_param->m_position));
    _param->m_changed = true;
  }
}

void dsp_host_dual::localTargetRcl(Target_Param *_param, const nltools::msg::ParameterGroups::MonoParameter &_source)
{
  _param->m_changed = false;
#if LOG_FAIL
  nltools::Log::warning("failed to recall modulateable MonoParameter mod-aspects!");
#endif
  //_param->update_source(getMacro(_source.mc));
  //_param->update_amount(static_cast<float>(_source.modulationAmount));
  if(_param->update_position(static_cast<float>(_source.controlPosition)))
  {
    _param->m_scaled = scale(_param->m_scaling, _param->polarize(_param->m_position));
    _param->m_changed = true;
  }
}
