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
    }
  }
}

void dsp_host_dual::onMidiMessage(const uint32_t _status, const uint32_t _data0, const uint32_t _data1)
{
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
      onMidiMessage(0xED, 0, _data0);            // keyPos
      onMidiMessage(0xEE, arg >> 7, arg & 127);  // keyDown
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
  // todo: glitch_suppression, stop envelopes on layer_changed
  nltools::Log::info("Received Single Preset Message!");
}

void dsp_host_dual::onPresetMessage(const nltools::msg::SplitPresetMessage &_msg)
{
  m_layer_changed = m_layer_mode != C15::Properties::LayerMode::Split;
  m_layer_mode = C15::Properties::LayerMode::Split;
  m_preloaded_split_data = _msg;
  // todo: glitch_suppression, stop envelopes on layer_changed
  nltools::Log::info("Received Split Preset Message!");
}

void dsp_host_dual::onPresetMessage(const nltools::msg::LayerPresetMessage &_msg)
{
  m_layer_changed = m_layer_mode != C15::Properties::LayerMode::Layer;
  m_layer_mode = C15::Properties::LayerMode::Layer;
  m_preloaded_layer_data = _msg;
  // todo: glitch_suppression, stop envelopes on layer_changed
  nltools::Log::info("Received Layer Preset Message!");
}

void dsp_host_dual::update_event_hw_source(const uint32_t _index, const bool _lock,
                                           const C15::Properties::HW_Return_Behavior _behavior, const float _position)
{
  auto param = m_params.get_source(_index);
  param->m_lock = _lock;
  param->m_behavior = _behavior;
  // behavior special cases ???
  if(param->m_position != _position)
  {
    param->m_position = _position;
    // trigger hw chain ...
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
    // silent mc update (base) ??? (most probably yes)
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
    param->update_modulation_aspects();
    // trigger mc chain ... (ribbon bidirectionality currently missing ...)
  }
}

void dsp_host_dual::update_event_macro_time(const uint32_t _index, const uint32_t _layer, const bool _lock,
                                            const float _position)
{
  auto param = m_params.get_macro(_layer, _index);
  param->m_lock = param->m_time.m_lock = _lock;  // currently, group lock affects both
  param->m_time.m_position = _position;
  // currently, no scaled field present for mc times ...
}

void dsp_host_dual::update_event_direct_param(const uint32_t _index, const uint32_t _layer, const bool _lock,
                                              const float _position)
{
  auto param = m_params.get_direct(_layer, _index);
  param->m_lock = _lock;
  if(param->m_position != _position)
  {
    param->m_position = _position;
    // trigger transition ...
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
  if(param->m_amount != _amount)
  {
    param->m_amount = _amount;
  }
  if(param->m_source != _source)
  {
    param->m_source = _source;
    m_params.m_layer[_layer].m_assignment.reassign(_index, srcId);
  }
  param->update_modulation_aspects(m_params.get_macro(_layer, srcId)->m_position);
  if(change)
  {
    // trigger transition ...
  }
}

void dsp_host_dual::update_event_global_param(const uint32_t _index, const bool _lock, const float _position)
{
  auto param = m_params.get_global(_index);
  param->m_lock = _lock;
  if(param->m_position != _position)
  {
    param->m_position = _position;
    // trigger transition ...
  }
}

void dsp_host_dual::keyDown(const float _vel)
{
  const bool valid = m_alloc.keyDown(m_key_pos);
  //for(auto keyEvent = m_alloc.m_traversal.first(); m_alloc.m_traversal.running(); keyEvent = m_alloc.m_traversal.next())
  //{}
}

void dsp_host_dual::keyUp(const float _vel)
{
  const bool valid = m_alloc.keyUp(m_key_pos);
  //for(auto keyEvent = m_alloc.m_traversal.first(); m_alloc.m_traversal.running(); keyEvent = m_alloc.m_traversal.next())
  //{}
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
}

float dsp_host_dual::scale(const C15::Properties::SmootherScale _id, const float _scaleFactor, const float _scaleOffset,
                           float _value)
{
  float result = 0.0f;
  switch(_id)
  {
    case C15::Properties::SmootherScale::None:
      break;
    case C15::Properties::SmootherScale::Linear:
      result = _scaleOffset + (_scaleFactor * _value);
      break;
    case C15::Properties::SmootherScale::Parabolic:
      result = _scaleOffset + (_scaleFactor * _value * std::abs(_value));
      break;
    case C15::Properties::SmootherScale::Cubic:
      result = _scaleOffset + (_scaleFactor * _value * _value * _value);
      break;
    case C15::Properties::SmootherScale::S_Curve:
      _value = (2.0f * (1.0f - _value)) - 1.0f;
      result = _scaleOffset + (_scaleFactor * ((_value * _value * _value * -0.25f) + (_value * 0.75f) + 0.5f));
      break;
    case C15::Properties::SmootherScale::Expon_Gain:
      result = m_convert.eval_level(_scaleOffset + (_scaleFactor * _value));
      break;
    case C15::Properties::SmootherScale::Expon_Osc_Pitch:
      result = m_convert.eval_osc_pitch(_scaleOffset + (_scaleFactor * _value));
      break;
    case C15::Properties::SmootherScale::Expon_Lin_Pitch:
      result = m_convert.eval_lin_pitch(_scaleOffset + (_scaleFactor * _value));
      break;
    case C15::Properties::SmootherScale::Expon_Shaper_Drive:
      result = (m_convert.eval_level(_value * _scaleFactor) * _scaleOffset) - _scaleOffset;
      break;
    case C15::Properties::SmootherScale::Expon_Mix_Drive:
      result = _scaleOffset + (_scaleFactor * m_convert.eval_level(_value));
      break;
    case C15::Properties::SmootherScale::Expon_Env_Time:
      result = m_convert.eval_time((_value * _scaleFactor * 104.0781f) + _scaleOffset);
      break;
  }
  return result;
}
