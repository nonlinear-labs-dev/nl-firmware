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
  m_layer_mode = C15::Properties::LayerMode::Single;
}

void dsp_host_dual::init(const uint32_t _samplerate, const uint32_t _polyphony)
{
  const float samplerate = static_cast<float>(_samplerate);
  m_alloc.init(&m_layer_mode);
  m_convert.init();
  m_clock.init(_samplerate);
  m_time.init(_samplerate);
  m_fade.init(samplerate);
  // init parameters by parameter list
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
  nltools::Log::info("dsp_host_dual::init");
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
      return C15::Parameters::Macro_Controls::MC_C;
    case MacroControls::MC6:
      return C15::Parameters::Macro_Controls::MC_D;
#endif
    default:
      return C15::Parameters::Macro_Controls::None;
  }
  // maybe, a simple static_cast would be sufficient ...
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
        // ...
        break;
      case 1:
        // Pedal 2
        arg = _data1 + (_data0 << 7);
        // ...
        break;
      case 2:
        // Pedal 3
        arg = _data1 + (_data0 << 7);
        // ...
        break;
      case 3:
        // Pedal 4
        arg = _data1 + (_data0 << 7);
        // ...
        break;
      case 4:
        // Bender
        arg = _data1 + (_data0 << 7);
        // ...
        break;
      case 5:
        // Aftertouch
        arg = _data1 + (_data0 << 7);
        // ...
        break;
      case 6:
        // Ribbon 1
        arg = _data1 + (_data0 << 7);
        // ...
        break;
      case 7:
        // Ribbon 2
        arg = _data1 + (_data0 << 7);
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
        // ...
        break;
      case 15:
        // Key Up
        arg = _data1 + (_data0 << 7);
        if(m_key_valid)
        {
          keyUp(static_cast<float>(arg) * m_norm_vel);
        }
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
  m_layer_mode = C15::Properties::LayerMode::Single;
  m_preloaded_single_data = _msg;
#if LOG_RECALL
  nltools::Log::info("Received Single Preset Message! (@", m_clock.m_index, ")");
#endif
  if(m_glitch_suppression)
  {
    // todo: glitch_suppression: fadepoint stuff ...
  }
  else
  {
    recall_event_single();
  }
}

void dsp_host_dual::onPresetMessage(const nltools::msg::SplitPresetMessage &_msg)
{
  m_layer_changed = m_layer_mode != C15::Properties::LayerMode::Split;
  m_layer_mode = C15::Properties::LayerMode::Split;
  m_preloaded_split_data = _msg;
#if LOG_RECALL
  nltools::Log::info("Received Split Preset Message!, (@", m_clock.m_index, ")");
#endif
  // todo: glitch_suppression, stop envelopes on layer_changed or unison_changed
}

void dsp_host_dual::onPresetMessage(const nltools::msg::LayerPresetMessage &_msg)
{
  m_layer_changed = m_layer_mode != C15::Properties::LayerMode::Layer;
  m_layer_mode = C15::Properties::LayerMode::Layer;
  m_preloaded_layer_data = _msg;
#if LOG_RECALL
  nltools::Log::info("Received Layer Preset Message!, (@", m_clock.m_index, ")");
#endif
  // todo: glitch_suppression, stop envelopes on layer_changed or unison_changed
}

void dsp_host_dual::update_event_hw_source(const uint32_t _index, const bool _lock,
                                           const C15::Properties::HW_Return_Behavior _behavior, const float _position)
{
  auto param = m_params.get_source(_index);
  param->m_lock = _lock;
  if(param->m_behavior != _behavior)
  {
    param->m_behavior = _behavior;
    param->m_position = _position;
#if LOG_EDITS
    nltools::Log::info("hw_edit(pos:", _position, ", behavior:", static_cast<int>(_behavior), ")");
#endif
    // todo: trigger mod-matrix re-evaluation ...
    // behavior logic (depending on source type: pedals or ribbons)
  }
  else if(param->m_position != _position)
  {
    param->m_position = _position;
#if LOG_EDITS
    nltools::Log::info("hw_edit(pos:", _position, ")");
#endif
    // todo: trigger hw chain ...
  }
}

void dsp_host_dual::update_event_hw_amount(const uint32_t _index, const uint32_t _layer, const bool _lock,
                                           const float _position)
{
  auto param = m_params.get_amount(_layer, _index);
  param->m_lock = _lock;
  if(param->m_position != _position)
  {
    param->m_position = _position;
#if LOG_EDITS
    nltools::Log::info("hw_amt(raw:", _position, ")");
#endif
    // todo: silent mc update (base) ...
  }
}

void dsp_host_dual::update_event_macro_ctrl(const uint32_t _index, const uint32_t _layer, const bool _lock,
                                            const float _position)
{
  auto param = m_params.get_macro(_layer, _index);
  param->m_lock = param->m_time.m_lock = _lock;  // currently, group lock affects both
  if(param->m_position != _position)
  {
    param->m_position = _position;
#if LOG_EDITS
    nltools::Log::info("mc_pos(raw:", _position, ")");
#endif
    param->update_modulation_aspects();
    // ribbon bidirectionality currently missing ...
    // trigger mc chain ...
    mod_event_mc_chain(_index, _layer, _position, param->m_time.m_dx);
  }
}

void dsp_host_dual::update_event_macro_time(const uint32_t _index, const uint32_t _layer, const bool _lock,
                                            const float _position)
{
  auto param = m_params.get_macro(_layer, _index);
  param->m_lock = param->m_time.m_lock = _lock;  // currently, group lock affects both
  if(param->m_time.m_position != _position)
  {
    param->m_time.m_position = _position;
    const float time = scale(param->m_time.m_scaling, param->m_time.m_position);
    update_event_time(&param->m_time.m_dx, time);
    // should update both layers in single mode...?
#if LOG_TIMES
    nltools::Log::info("MC_time(id:", _index, ", layer:", _layer, "raw:", _position, ",time:", time, ")");
#endif
  }
}

void dsp_host_dual::update_event_direct_param(const uint32_t _index, const uint32_t _layer, const bool _lock,
                                              const float _position)
{
  auto param = m_params.get_direct(_layer, _index);
  param->m_lock = _lock;
  const bool change = param->m_position != _position;
  if(change)
  {
    param->m_position = _position;
    if(_index == static_cast<uint32_t>(C15::Parameters::Unmodulateable_Parameters::Unison_Voices))
    {
#if LOG_RESET
      nltools::Log::info("Unison Voices changed!");
#endif
      // reset voice allocation, stop envelopes ...
    }
    else
    {
      const float dest = scale(param->m_scaling, param->m_position);
#if LOG_EDITS
      nltools::Log::info("dp_pos(raw:", _position, ", ", "dest:", dest, ")");
#endif
      // trigger transition (edit time) ...
      transition_event(param->m_renderIndex, _layer, m_edit_time, param->m_section, param->m_clock, dest);
    }
  }
}

void dsp_host_dual::update_event_target_param(const uint32_t _index, const uint32_t _layer, const bool _lock,
                                              const float _position, const C15::Parameters::Macro_Controls _source,
                                              const float _amount)
{
  auto param = m_params.get_target(_layer, _index);
  const float pos = param->depolarize(_position);
  const bool change = param->m_position != pos;
  const uint32_t srcId = static_cast<uint32_t>(_source);
  param->m_lock = _lock;
  param->m_position = pos;
  param->m_amount = _amount;
  if(param->m_source != _source)
  {
    param->m_source = _source;
    m_params.m_layer[_layer].m_assignment.reassign(_index, srcId);
  }
  param->update_modulation_aspects(m_params.get_macro(_layer, srcId)->m_position);
  if(change)
  {
    const float dest = scale(param->m_scaling, param->polarize(param->m_position));
#if LOG_EDITS
    nltools::Log::info("tp_pos(raw:", _position, "dest:", dest, ")");
#endif
    // trigger transition (edit time) ...
    transition_event(param->m_renderIndex, _layer, m_edit_time, param->m_section, param->m_clock, dest);
  }
}

void dsp_host_dual::update_event_global_param(const uint32_t _index, const bool _lock, const float _position)
{
  auto param = m_params.get_global(_index);
  param->m_lock = _lock;
  if(param->m_position != _position)
  {
    param->m_position = _position;
    const float dest = scale(param->m_scaling, param->m_position);
#if LOG_EDITS
    nltools::Log::info("gp_pos(raw:", _position, "dest:", dest, ")");
#endif
    // trigger transition (edit time) ...
    transition_event(param->m_renderIndex, m_edit_time, param->m_section, param->m_clock, dest);
  }
}

void dsp_host_dual::update_event_edit_time(const float _position)
{
  const float time = 200.0f * _position;
  update_event_time(&m_edit_time, time);
#if LOG_TIMES
  nltools::Log::info("edit_time(raw:", _position, ", time:", time, ")");
#endif
}

void dsp_host_dual::update_event_transition_time(const float _position)
{
  // transition time scales like mc smoothing or like finite env times
  const float time = scale(m_transition_scale, _position);
  update_event_time(&m_transition_time, time);
#if LOG_TIMES
  nltools::Log::info("transition_time(raw:", _position, ", time:", time, ")");
#endif
}

void dsp_host_dual::update_event_note_shift(const float _shift)
{
  m_global.m_note_shift = _shift;
#if LOG_SETTINGS
  nltools::Log::info("note_shift:", _shift);
#endif
}

void dsp_host_dual::update_event_glitch_suppr(const bool _enabled)
{
  m_glitch_suppression = _enabled;
#if LOG_SETTINGS
  nltools::Log::info("glitch_suppression:", _enabled);
#endif
}

void dsp_host_dual::render()
{
  m_clock.render();
  // slow rendering
  if(m_clock.m_slow)
  {
  }
  // fast rendering
  if(m_clock.m_fast)
  {
  }
  // audio rendering (always)
}

void dsp_host_dual::reset()
{
#if LOG_RESET
  nltools::Log::info("dsp_reset()");
#endif
}

void dsp_host_dual::keyDown(const float _vel)
{
  const bool valid = m_alloc.keyDown(m_key_pos);
  //for(auto keyEvent = m_alloc.m_traversal.first(); m_alloc.m_traversal.running(); keyEvent = m_alloc.m_traversal.next())
  //{}
#if LOG_KEYS
  nltools::Log::info("key_down()");
#endif
}

void dsp_host_dual::keyUp(const float _vel)
{
  const bool valid = m_alloc.keyUp(m_key_pos);
  //for(auto keyEvent = m_alloc.m_traversal.first(); m_alloc.m_traversal.running(); keyEvent = m_alloc.m_traversal.next())
  //{}
#if LOG_KEYS
  nltools::Log::info("key_up()");
#endif
}

float dsp_host_dual::scale(const Parameter_Scale<C15::Properties::SmootherScale> _scl, float _value)
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

void dsp_host_dual::update_event_time(Time_Parameter *_param, const float _ms)
{
  m_time.update_ms(_ms);
  _param->m_dx_audio = m_time.m_dx_audio;
  _param->m_dx_fast = m_time.m_dx_fast;
  _param->m_dx_slow = m_time.m_dx_slow;
}

void dsp_host_dual::mod_event_mc_chain(const uint32_t _index, const uint32_t _layer, const float _position,
                                       const Time_Parameter _time)
{
  auto list = m_params.m_layer[_layer].m_assignment;
  for(auto id = list.first(_index); list.running(); id = list.next())
  {
    auto param = m_params.get_target(_layer, id);
    if(param->modulate(_position))
    {
      param->m_position = param->m_unclipped < 0.0f ? 0.0f : param->m_unclipped > 1.0f ? 1.0f : param->m_unclipped;
      const float dest = scale(param->m_scaling, param->polarize(param->m_position));
      transition_event(param->m_renderIndex, _layer, _time, param->m_section, param->m_clock, dest);
    }
  }
}

void dsp_host_dual::transition_event(const uint32_t _id, const uint32_t _layer, const Time_Parameter _time,
                                     const C15::Descriptors::SmootherSection _section,
                                     const C15::Descriptors::SmootherClock _clock, const float _dest)
{
  switch(_section)
  {
    case C15::Descriptors::SmootherSection::Poly:
      switch(_clock)
      {
        case C15::Descriptors::SmootherClock::Sync:
          m_poly[_layer].start_sync(_id, _dest);
          break;
        case C15::Descriptors::SmootherClock::Audio:
          m_poly[_layer].start_audio(_id, _time.m_dx_audio, _dest);
          break;
        case C15::Descriptors::SmootherClock::Fast:
          m_poly[_layer].start_fast(_id, _time.m_dx_fast, _dest);
          break;
        case C15::Descriptors::SmootherClock::Slow:
          m_poly[_layer].start_slow(_id, _time.m_dx_slow, _dest);
          break;
      }
      break;
    case C15::Descriptors::SmootherSection::Mono:
      switch(_clock)
      {
        case C15::Descriptors::SmootherClock::Sync:
          m_mono[_layer].start_sync(_id, _dest);
          break;
        case C15::Descriptors::SmootherClock::Audio:
          m_mono[_layer].start_audio(_id, _time.m_dx_audio, _dest);
          break;
        case C15::Descriptors::SmootherClock::Fast:
          m_mono[_layer].start_fast(_id, _time.m_dx_fast, _dest);
          break;
        case C15::Descriptors::SmootherClock::Slow:
          m_mono[_layer].start_slow(_id, _time.m_dx_slow, _dest);
          break;
      }
      break;
  }
#if LOG_TRANSITIONS
  nltools::Log::info("local_transition(id:", _id, ", dest:", _dest, ")");
#endif
}

void dsp_host_dual::transition_event(const uint32_t _id, const Time_Parameter _time,
                                     const C15::Descriptors::SmootherSection _section,
                                     const C15::Descriptors::SmootherClock _clock, const float _dest)
{
  switch(_section)
  {
    case C15::Descriptors::SmootherSection::Global:
      switch(_clock)
      {
        case C15::Descriptors::SmootherClock::Sync:
          m_global.start_sync(_id, _dest);
          break;
        case C15::Descriptors::SmootherClock::Audio:
          m_global.start_audio(_id, _time.m_dx_audio, _dest);
          break;
        case C15::Descriptors::SmootherClock::Fast:
          m_global.start_fast(_id, _time.m_dx_fast, _dest);
          break;
        case C15::Descriptors::SmootherClock::Slow:
          m_global.start_slow(_id, _time.m_dx_slow, _dest);
          break;
      }
      break;
  }
#if LOG_TRANSITIONS
  nltools::Log::info("global_transition(id:", _id, ", dest:", _dest, ")");
#endif
}

bool dsp_host_dual::recall_event_changed(const C15::Properties::LayerId _layerId, const float _value)
{
  bool reset = m_layer_changed;
  auto param = m_params.get(_layerId, C15::Parameters::Unmodulateable_Parameters::Unison_Voices);
  if(!param->m_lock)
  {
    reset |= (param->m_position != _value);
    param->m_position = _value;
    const float dest = scale(param->m_scaling, param->m_position);
    const uint32_t layerId = static_cast<uint32_t>(_layerId);
    transition_event(param->m_renderIndex, layerId, m_transition_time, param->m_section, param->m_clock, dest);
  }
  return reset;
}

void dsp_host_dual::recall_event_global(const nltools::msg::ParameterGroups::PedalParameter &source)
{
#if LOG_DISPATCH
  nltools::Log::info("dispatch_hw_source(", source.id, ")");
#endif
  auto target = m_params.get_source(C15::ParameterList[source.id].m_param.m_index);
  // target->m_lock appears to be redundant, so locking can disappear from parameter changes messages
  if(!source.locked)
  {
    target->m_behavior = getBehavior(source.returnMode);  // msg: pedal mode?
    // (audio engine only requires one return type for all sources)
    target->m_position = source.controlPosition;
  }
}

void dsp_host_dual::recall_event_global(const nltools::msg::ParameterGroups::BenderParameter &source)
{
#if LOG_DISPATCH
  nltools::Log::info("dispatch_hw_source(", source.id, ")");
#endif
  auto target = m_params.get_source(C15::ParameterList[source.id].m_param.m_index);
  // target->m_lock appears to be redundant, so locking can disappear from parameter changes messages
  if(!source.locked)
  {
    target->m_behavior = getBehavior(source.returnMode);
    // seems obsolete, since bender behavior is constant
    target->m_position = source.controlPosition;
  }
}

void dsp_host_dual::recall_event_global(const nltools::msg::ParameterGroups::AftertouchParameter &source)
{
#if LOG_DISPATCH
  nltools::Log::info("dispatch_hw_source(", source.id, ")");
#endif
  auto target = m_params.get_source(C15::ParameterList[source.id].m_param.m_index);
  // target->m_lock appears to be redundant, so locking can disappear from parameter changes messages
  if(!source.locked)
  {
    target->m_behavior = getBehavior(source.returnMode);
    // seems obsolete, since aftertouch behavior is constant
    target->m_position = source.controlPosition;
  }
}

void dsp_host_dual::recall_event_global(const nltools::msg::ParameterGroups::RibbonParameter &source)
{
#if LOG_DISPATCH
  nltools::Log::info("dispatch_hw_source(", source.id, ")");
#endif
  auto target = m_params.get_source(C15::ParameterList[source.id].m_param.m_index);
  // target->m_lock appears to be redundant, so locking can disappear from parameter changes messages
  if(!source.locked)
  {
    target->m_behavior = getBehavior(source.ribbonReturnMode);  // msg: ribbonReturnMode?
    // (audio engine only requires one return type for all sources)
    target->m_position = source.controlPosition;
  }
}

void dsp_host_dual::recall_event_global(const nltools::msg::ParameterGroups::Parameter &source)
{
#if LOG_DISPATCH
  nltools::Log::info("dispatch_global(", source.id, ")");
#endif
  auto target = m_params.get_global(C15::ParameterList[source.id].m_param.m_index);
  // target->m_lock appears to be redundant, so locking can disappear from parameter changes messages
  if(!source.locked)
  {
    target->m_position = source.controlPosition;
    const float dest = scale(target->m_scaling, target->m_position);
    transition_event(target->m_renderIndex, m_transition_time, target->m_section, target->m_clock, dest);
  }
}

void dsp_host_dual::recall_event_local(const uint32_t layer, const nltools::msg::ParameterGroups::MonoParameter &source)
{
  // could be locked
  if(!source.locked)
  {
    // could be direct or target
#if LOG_DISPATCH
    nltools::Log::info("dispatch_mono(", source.id, ")");
#endif
    auto param = C15::ParameterList[source.id];
    switch(param.m_param.m_type)
    {
      case C15::Descriptors::ParameterType::Unmodulateable_Parameter:
        recall_event_direct_param(layer, param.m_param.m_index, source.controlPosition);
        break;
      case C15::Descriptors::ParameterType::Modulateable_Parameter:
        // bug: MonoParameter doesn't feature aspects yet (should be part of modulateables/unmodulateables) ...
        recall_event_target_param(layer, param.m_param.m_index, source.controlPosition,
                                  C15::Parameters::Macro_Controls::None, 0.0f);
        // hack: just update control pos, fake rest
        break;
    }
  }
}

void dsp_host_dual::recall_event_local(const uint32_t layer,
                                       const nltools::msg::ParameterGroups::MacroParameter &source)
{
#if LOG_DISPATCH
  nltools::Log::info("dispatch_macro(", source.id, ")");
#endif
  auto target = m_params.get_macro(layer, C15::ParameterList[source.id].m_param.m_index);
  // target->m_lock appears to be redundant, so locking can disappear from parameter changes messages
  if(!source.locked)
  {
    target->m_position = source.controlPosition;
    target->update_modulation_aspects();
  }
}

void dsp_host_dual::recall_event_local(const uint32_t layer,
                                       const nltools::msg::ParameterGroups::UnmodulatebaleParameter &source)
{
#if LOG_DISPATCH
  nltools::Log::info("dispatch_unmodulateable(", source.id, ")");
#endif
  if(!source.locked)
  {
    recall_event_direct_param(layer, C15::ParameterList[source.id].m_param.m_index, source.controlPosition);
  }
}

void dsp_host_dual::recall_event_local(const uint32_t layer,
                                       const nltools::msg::ParameterGroups::ModulateableParameter &source)
{
#if LOG_DISPATCH
  nltools::Log::info("dispatch_modulateable(", source.id, ")");
#endif
  if(!source.locked)
  {
    recall_event_target_param(layer, C15::ParameterList[source.id].m_param.m_index, source.controlPosition,
                              getMacro(source.mc), source.modulationAmount);
  }
  recall_event_target_aspects(layer, C15::ParameterList[source.id].m_param.m_index);
}

void dsp_host_dual::recall_event_direct_param(const uint32_t _layer, const uint32_t _index, const float _position)
{
  auto target = m_params.get_direct(_layer, _index);
  target->m_position = _position;
  const float dest = scale(target->m_scaling, target->m_position);
  transition_event(target->m_renderIndex, _layer, m_transition_time, target->m_section, target->m_clock, dest);
}
void dsp_host_dual::recall_event_target_param(const uint32_t _layer, const uint32_t _index, const float _position,
                                              const C15::Parameters::Macro_Controls _source, const float _amount)
{
  auto target = m_params.get_target(_layer, _index);
  target->m_position = _position;
  target->m_source = _source;
  target->m_amount = _amount;
  const float dest = scale(target->m_scaling, target->m_position);
  transition_event(target->m_renderIndex, _layer, m_transition_time, target->m_section, target->m_clock, dest);
}

void dsp_host_dual::recall_event_target_aspects(const uint32_t _layer, const uint32_t _index)
{
  auto target = m_params.get_target(_layer, _index);
  const uint32_t srcId = static_cast<uint32_t>(target->m_source);
  m_params.m_layer[_layer].m_assignment.reassign(_index, srcId);
  target->update_modulation_aspects(m_params.get_macro(_layer, srcId)->m_position);
}

void dsp_host_dual::recall_event_single()
{
#if LOG_RECALL
  nltools::Log::info("Recall_Single! (@", m_clock.m_index, ")");
#endif
  auto data = &m_preloaded_single_data;  // shorthand reference
  // reset detection and potential unison voices update
  if(recall_event_changed(C15::Properties::LayerId::I, data->unisonVoices.controlPosition))
  {
    // reset voice allocation and envelopes (fade point is zero at this time when glitch suppression is active)
    m_alloc.reset();  // clear all keys
    for(auto key = m_alloc.m_traversal.first(); m_alloc.m_traversal.running(); key = m_alloc.m_traversal.next())
    {
      // todo: stop envelopes (abruptly) by key->localIndex, key->voiceId (poly_section)
    }
    // apply unison to voice allocation
    m_alloc.setUnison(AllocatorId::Global, data->unisonVoices.controlPosition);
#if LOG_RECALL
    nltools::Log::info("reset detected");
#endif
  }
  // reset mc assignments (both voice groups)
  m_params.m_layer[0].m_assignment.reset();
  m_params.m_layer[1].m_assignment.reset();
  // inconvenient preset msg architecture requires many iterations ...
  // - global / hw sources - pedals , bender, aftertouch, ribbons (one array within msg would be convenient)
  for(uint32_t i = 0; i < data->pedals.size(); i++)
  {
    recall_event_global(data->pedals[i]);
  }
  for(uint32_t i = 0; i < data->bender.size(); i++)
  {
    recall_event_global(data->bender[i]);
  }
  for(uint32_t i = 0; i < data->aftertouch.size(); i++)
  {
    recall_event_global(data->aftertouch[i]);
  }
  for(uint32_t i = 0; i < data->ribbons.size(); i++)
  {
    recall_event_global(data->ribbons[i]);
  }
  // - global / parameters - master, scale (one global array within msg would be convenient)
  for(uint32_t i = 0; i < data->master.size(); i++)
  {
    recall_event_global(data->master[i]);
  }
  for(uint32_t i = 0; i < data->scale.size(); i++)
  {
    recall_event_global(data->scale[i]);
  }
  // - local / multiple (according to msg) - macros, unmodulateables, modulateables, mono (dispatch needed)
  for(uint32_t l = 0; l < static_cast<uint32_t>(C15::Properties::LayerId::_LENGTH_); l++)
  {
    // maybe at first unmodulateables (containing hw amounts, mc times)
    for(uint32_t i = 0; i < data->unmodulateables.size(); i++)
    {
      recall_event_local(l, data->unmodulateables[i]);
    }
    // now, hw sources and amounts and mc times are set!
    // todo: mc bases can be determined here ...
    // continue with macro controls, they can update mod aspects now!
    for(uint32_t i = 0; i < data->macros.size(); i++)
    {
      recall_event_local(l, data->macros[i]);
    }
    // mono parameters cause trouble - invalid parameter ids in pg msg!!!
#if IGNORE_MONO_GROUP
#warning                                                                                                               \
    "nltools::message.h : Mono Group (and possibly Voice Group) Params have invalid ids! (use ParameterList_1.7B.ods)"
#else
    // continue with remaining parameters, they can update mod aspects now!
    for(uint32_t i = 0; i < data->monos.size(); i++)
    {
      recall_event_local(l, data->monos[i]);
    }
#endif
    for(uint32_t i = 0; i < data->modulateables.size(); i++)
    {
      recall_event_local(l, data->modulateables[i]);
    }
  }
}
