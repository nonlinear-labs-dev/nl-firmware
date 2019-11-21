#pragma once

/******************************************************************************/
/** @file       parameter_handle.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      new main parameter handle
    @todo
*******************************************************************************/

#include "parameter_storage.h"
#include "parameter-db/generated/parameter_list.h"

struct ClockHandle
{
  uint32_t m_index = 0, m_div_fast = 0, m_div_slow = 0;
  bool m_fast = false, m_slow = false;
  inline void init(const uint32_t _samplerate)
  {
    m_div_fast = _samplerate / C15::Config::clock_rates[0][1];
    m_div_slow = _samplerate / C15::Config::clock_rates[0][2];
  }
  inline void render()
  {
    m_fast = (m_index % m_div_fast) == 0;
    m_index = m_index % m_div_slow;
    m_slow = m_index == 0;
    m_index++;
  }
};

struct TimeHandle
{
  float m_convert[4] = {}, m_dx_audio = 0.0f, m_dx_fast = 0.0f, m_dx_slow = 0.0f, m_millisecond = 0.0f,
        m_sample_inc = 0.0f;
  inline void init(const uint32_t _samplerate)
  {
    float rate = static_cast<float>(_samplerate);
    m_millisecond = 1e-3f * rate;
    m_sample_inc = 1.0f / rate;
    m_convert[1] = 1.0f;
    m_convert[2] = rate / static_cast<float>(C15::Config::clock_rates[0][1]);
    m_convert[3] = rate / static_cast<float>(C15::Config::clock_rates[0][2]);
  }
  inline float clip(const float _value)
  {
    return _value > 1.0f ? 1.0f : _value;
  }
  inline float eval_ms(const uint32_t _type, const float _value)
  {
    return clip(m_convert[_type] / ((_value * m_millisecond) + 1.0f));
  }
  inline void update_ms(const float _value)
  {
    m_dx_audio = eval_ms(1, _value);
    m_dx_fast = eval_ms(2, _value);
    m_dx_slow = eval_ms(3, _value);
  }
};

template <typename Scale, typename Section, typename Clock, typename Signal, class Layer, class HW, class GP, class HA,
          class MC, class TP, class DP>
struct ParameterHandle
{
  // init
  inline void init_macro(const C15::ParameterDescriptor _element)
  {
    for(uint32_t i = 0; i < m_layer_count; i++)
    {
      auto param = get_macro(i, _element.m_param.m_index);
      param->m_index = m_source_count * (_element.m_param.m_index - 1);
      param->m_base = param->m_position = _element.m_initial;
    }
  }
  inline void init_macro_time(const C15::ParameterDescriptor _element)
  {
    for(uint32_t i = 0; i < m_layer_count; i++)
    {
      auto param = get_macro(i, _element.m_param.m_index);
      param->m_time.m_scaleId = _element.m_ae.m_scaleId;
      param->m_time.m_scaleFactor = _element.m_ae.m_scaleFactor;
      param->m_time.m_scaleOffset = _element.m_ae.m_scaleOffset;
    }
  }
  inline void init_modulateable(const C15::ParameterDescriptor _element)
  {
    for(uint32_t i = 0; i < m_layer_count; i++)
    {
      auto param = get_target(i, _element.m_param.m_index);
      param->m_polarity = _element.m_ae.m_polarity;
      param->m_section = _element.m_ae.m_smoother.m_section;
      param->m_clock = _element.m_ae.m_smoother.m_clock;
      param->m_renderIndex = _element.m_ae.m_smoother.m_index;
      param->m_scaleId = _element.m_ae.m_scaleId;
      param->m_scaleFactor = _element.m_ae.m_scaleFactor;
      param->m_scaleOffset = _element.m_ae.m_scaleOffset;
      param->m_position = param->depolarize(_element.m_initial);
    }
  }
  inline void init_unmodulateable(const C15::ParameterDescriptor _element)
  {
    for(uint32_t i = 0; i < m_layer_count; i++)
    {
      auto param = get_direct(i, _element.m_param.m_index);
      param->m_section = _element.m_ae.m_smoother.m_section;
      param->m_clock = _element.m_ae.m_smoother.m_clock;
      param->m_renderIndex = _element.m_ae.m_smoother.m_index;
      param->m_scaleId = _element.m_ae.m_scaleId;
      param->m_scaleFactor = _element.m_ae.m_scaleFactor;
      param->m_scaleOffset = _element.m_ae.m_scaleOffset;
      param->m_position = _element.m_initial;
    }
  }
  inline void init_global(const C15::ParameterDescriptor _element)
  {
    for(uint32_t i = 0; i < m_layer_count; i++)
    {
      auto param = get_global(_element.m_param.m_index);
      param->m_section = _element.m_ae.m_smoother.m_section;
      param->m_clock = _element.m_ae.m_smoother.m_clock;
      param->m_renderIndex = _element.m_ae.m_smoother.m_index;
      param->m_scaleId = _element.m_ae.m_scaleId;
      param->m_scaleFactor = _element.m_ae.m_scaleFactor;
      param->m_scaleOffset = _element.m_ae.m_scaleOffset;
      param->m_position = _element.m_initial;
    }
  }
  // numeric getters (when dispatching TCD ids)
  inline Hardware_Source_Parameter* get_source(const uint32_t _id)
  {
    return &m_global.m_source[_id];
  }
  inline Unmodulateable_Parameter<Scale, Section, Clock, Signal>* get_global(const uint32_t _id)
  {
    return &m_global.m_direct[_id];
  }
  inline Hardware_Amount_Parameter* get_amount(const uint32_t _layerId, const uint32_t _id)
  {
    return &m_layer[_layerId].m_amount[_id];
  }
  inline Macro_Control_Parameter<Scale>* get_macro(const uint32_t _layerId, const uint32_t _id)
  {
    return &m_layer[_layerId].m_macro[_id];
  }
  inline Modulateable_Parameter<Scale, Section, Clock, Signal>* get_target(const uint32_t _layerId, const uint32_t _id)
  {
    return &m_layer[_layerId].m_target[_id];
  }
  inline Unmodulateable_Parameter<Scale, Section, Clock, Signal>* get_direct(const uint32_t _layerId,
                                                                             const uint32_t _id)
  {
    return &m_layer[_layerId].m_direct[_id];
  }
  // enum-class-based getters (if needed)
  inline Hardware_Source_Parameter* get(const HW _id)
  {
    return &m_global.m_source[static_cast<uint32_t>(_id)];
  }
  inline Unmodulateable_Parameter<Scale, Section, Clock, Signal>* get(const GP _id)
  {
    return &m_global.m_direct[static_cast<uint32_t>(_id)];
  }
  inline Hardware_Amount_Parameter* get(const Layer _layerId, const HA _id)
  {
    return &m_layer[static_cast<uint32_t>(_layerId)].m_amount[static_cast<uint32_t>(_id)];
  }
  inline Macro_Control_Parameter<Scale>* get(const Layer _layerId, const MC _id)
  {
    return &m_layer[static_cast<uint32_t>(_layerId)].m_macro[static_cast<uint32_t>(_id)];
  }
  inline Modulateable_Parameter<Scale, Section, Clock, Signal>* get(const Layer _layerId, const TP _id)
  {
    return &m_layer[static_cast<uint32_t>(_layerId)].m_target[static_cast<uint32_t>(_id)];
  }
  inline Unmodulateable_Parameter<Scale, Section, Clock, Signal>* get(const Layer _layerId, const DP _id)
  {
    return &m_layer[static_cast<uint32_t>(_layerId)].m_direct[static_cast<uint32_t>(_id)];
  }
  // members: layered and global parameter storage
  Layer_Parameter_Storage<Scale, Section, Clock, Signal, HA, MC, TP, DP>
      m_layer[static_cast<uint32_t>(Layer::_LENGTH_)];
  Global_Parameter_Storage<Scale, Section, Clock, Signal, HW, GP> m_global;
  const uint32_t m_layer_count = static_cast<uint32_t>(Layer::_LENGTH_),
                 m_source_count = static_cast<uint32_t>(HW::_LENGTH_),
                 m_amount_count = static_cast<uint32_t>(HA::_LENGTH_),
                 m_macro_count = static_cast<uint32_t>(MC::_LENGTH_);
};
