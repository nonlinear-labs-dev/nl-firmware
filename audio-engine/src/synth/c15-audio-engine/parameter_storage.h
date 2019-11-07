#pragma once

/******************************************************************************/
/** @file       parameter_storage.h
    @date
    @version    1.7-0
    @author     M. Seeber
    @brief      abstractions for parameters
                (currently, all parameters operate completely event-based)
    @todo
*******************************************************************************/

#include "parameter-db/generated/c15_config.h"
#include "parameter-db/generated/parameter_info.h"
#include "macro_assignment.h"

// prototype
struct Proto_Parameter
{
  float m_position = 0.0f;
  bool m_lock = false;
};

// derived parameter types
struct Hardware_Source_Parameter : Proto_Parameter
{
  C15::Properties::HW_Return_Behavior m_behavior = C15::Properties::HW_Return_Behavior::Stay;
};

struct Hardware_Amount_Parameter : Proto_Parameter
{
};

struct Macro_Time_Parameter : Proto_Parameter
{
};

struct Macro_Control_Parameter : Proto_Parameter
{
  Macro_Time_Parameter m_time;
  float m_base = 0.0f, m_mod = 0.0f, m_unclipped = 0.0f;
  uint32_t m_index = 0;
  bool m_bidir = false;
  inline bool modulate(const float _mod)
  {
    m_mod = _mod;
    m_unclipped = m_base + m_mod;
    return m_position != m_unclipped;
  }
  inline void update_modulation_aspects()
  {
    m_base = m_position - m_mod;
  }
};

template <typename Scale, typename Section, typename Clock, typename Signal>
struct Modulateable_Parameter : Proto_Parameter
{
  float m_amount = 0.0f, m_base = 0.0f, m_ceil = 0.0f, m_scaleFactor = 0.0f, m_scaleOffset = 0.0f, m_unclipped = 0.0f;
  C15::Parameters::Macro_Controls m_source = C15::Parameters::Macro_Controls::None;
  Scale m_scaleId;
  Section m_section;
  Clock m_clock;
  uint32_t m_renderIndex = 0;
  bool m_polarity = false;
  inline float depolarize(const float _value)
  {
    if(m_polarity)
    {
      return (0.5f * _value) + 0.5f;
    }
    return _value;
  }
  inline float polarize(const float _value)
  {
    if(m_polarity)
    {
      return (2.0f * _value) - 1.0f;
    }
    return _value;
  }
  inline bool modulate(const float _mod)
  {
    m_unclipped = m_base + (m_amount * _mod);
    m_position = m_unclipped < 0.0f ? 0.0f : m_unclipped > 1.0f ? 1.0f : m_unclipped;
    return m_position != m_unclipped;
  }
  inline void update_modulation_aspects(const float _mod)
  {
    m_base = m_position - (m_amount * _mod);
    m_ceil = m_base + m_amount;
  }
};

template <typename Scale, typename Section, typename Clock, typename Signal>
struct Unmodulateable_Parameter : Proto_Parameter
{
  float m_scaleFactor = 0.0f, m_scaleOffset = 0.0f;
  Scale m_scaleId;
  Section m_section;
  Clock m_clock;
  uint32_t m_renderIndex;
};

// parameter storage objects
template <typename Scale, typename Section, typename Clock, typename Signal, class HW, class GP>
struct Global_Parameter_Storage
{
  Hardware_Source_Parameter m_source[static_cast<uint32_t>(HW::_LENGTH_)];
  Unmodulateable_Parameter<Scale, Section, Clock, Signal> m_direct[static_cast<uint32_t>(GP::_LENGTH_)];
};

template <typename Scale, typename Section, typename Clock, typename Signal, class HA, class MC, class TP, class DP>
struct Layer_Parameter_Storage
{
  Hardware_Amount_Parameter m_amount[static_cast<uint32_t>(HA::_LENGTH_)];
  Macro_Control_Parameter m_macro[static_cast<uint32_t>(MC::_LENGTH_)];
  Modulateable_Parameter<Scale, Section, Clock, Signal> m_target[static_cast<uint32_t>(TP::_LENGTH_)];
  Unmodulateable_Parameter<Scale, Section, Clock, Signal> m_direct[static_cast<uint32_t>(DP::_LENGTH_)];
  MC_Assignment<MC, TP> m_assignment;
};
