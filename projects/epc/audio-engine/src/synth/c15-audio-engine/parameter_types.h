#pragma once

#include <Parameters.h>

namespace Engine
{

  namespace Parameters
  {

    // new approach via composition
    namespace Aspects
    {

      struct ParameterAspect
      {
        float m_position = {}, m_initial = {};
        inline bool update_position(const float &_position)
        {
          if(_position != m_position)
          {
            m_position = _position;
            return true;
          }
          return false;
        }
        inline void init(const float &_initial)
        {
            m_initial = _initial;
            m_position = _initial;
        }
      };

      struct RangeAspect
      {
        float m_unclipped = {};
      };

      struct ModulationAspect : public RangeAspect
      {
        float m_amount = {}, m_base = {}, m_ceil = {};
        C15::Parameters::Macro_Controls m_source = C15::Parameters::Macro_Controls::None;
        bool m_polarity = false;
        inline void init(const bool &_polarity)
        {
            m_polarity = _polarity;
        }
        inline bool update_source(const C15::Parameters::Macro_Controls &_source)
        {
          if(_source != m_source)
          {
            m_source = _source;
            return true;
          }
          return false;
        }
        inline bool update_amount(const float &_amount)
        {
          if(_amount != m_amount)
          {
            m_amount = _amount;
            return true;
          }
          return false;
        }
        inline float depolarize(const float &_value) const
        {
          if(m_polarity)
          {
            return (0.5f * _value) + 0.5f;
          }
          return _value;
        }
        inline float polarize(const float &_value) const
        {
          if(m_polarity)
          {
            return (2.0f * _value) - 1.0f;
          }
          return _value;
        }
      };

      struct ScaleAspect
      {
        C15::Properties::SmootherScale m_scaleId;
        float m_scaleFactor = {}, m_scaleOffset = {}, m_scaled = {};
        inline void init(const C15::Descriptors::AudioEngineParamDescriptor &_desc)
        {
            m_scaleId = _desc.m_scaleId;
            m_scaleFactor = _desc.m_scaleFactor;
            m_scaleOffset = _desc.m_scaleOffset;
        }
      };

      struct TimeAspect
      {
        float m_dxAudio = {}, m_dxFast = {}, m_dxSlow = {};
      };

      struct RenderAspect
      {
        C15::Descriptors::SmootherSection m_renderSection;
        C15::Descriptors::SmootherClock m_renderClock;
        uint32_t m_renderIndex = {};
        inline void init(const C15::Descriptors::SmootherDescriptor &_desc)
        {
            m_renderSection = _desc.m_section;
            m_renderClock = _desc.m_clock;
            m_renderIndex = _desc.m_index;
        }
      };

    }  // namespace Engine::Parameters::Aspects

    // usable parameters

    struct HardwareSource : public Aspects::ParameterAspect
    {
      C15::Properties::HW_Return_Behavior m_behavior = C15::Properties::HW_Return_Behavior::Stay;
      uint32_t m_offset = {};
      inline bool update_behavior(const C15::Properties::HW_Return_Behavior &_behavior)
      {
        if(_behavior != m_behavior)
        {
          m_behavior = _behavior;
          return true;
        }
        return false;
      }
    };

    struct HardwareAmount : public Aspects::ParameterAspect
    {
      uint32_t m_sourceId = {};
    };

    struct MacroControl : public Aspects::ParameterAspect, public Aspects::RangeAspect
    {
      uint32_t m_id = {}, m_index = {};
      inline void init(const C15::ParameterDescriptor &_desc)
      {
          ParameterAspect::init(_desc.m_initial);
          m_id = _desc.m_param.m_index;
          m_index = _desc.m_param.m_index;
      }
    };

    struct MacroTime : public Aspects::ParameterAspect, public Aspects::ScaleAspect, public Aspects::TimeAspect
    {
        inline void init(const C15::ParameterDescriptor &_desc)
        {
            ParameterAspect::init(_desc.m_initial);
            ScaleAspect::init(_desc.m_ae);
        }
    };

    struct Modulateable : public Aspects::ParameterAspect,
                          public Aspects::ModulationAspect,
                          public Aspects::ScaleAspect,
                          public Aspects::RenderAspect
    {
      bool m_splitpoint = false;
      inline void init(const C15::ParameterDescriptor &_desc)
      {
          ParameterAspect::init(depolarize(_desc.m_initial));
          ModulationAspect::init(_desc.m_ae.m_polarity);
          ScaleAspect::init(_desc.m_ae);
          RenderAspect::init(_desc.m_ae.m_smoother);
          m_splitpoint = _desc.m_param_id == C15::PID::Split_Split_Point;
      }
      inline bool modulate(const float &_mod)
      {
        m_unclipped = m_base + (m_amount * _mod);
        return m_position != m_unclipped;  // basic change detection (position needs to be updated from outside)
      }
      inline void update_modulation_aspects(const float &_mod)
      {
        m_base = m_position - (m_amount * _mod);
        m_ceil = m_base + m_amount;
      }
    };

    struct Unmodulateable : public Aspects::ParameterAspect, public Aspects::ScaleAspect, public Aspects::RenderAspect
    {
        inline void init(const C15::ParameterDescriptor &_desc)
        {
            ParameterAspect::init(_desc.m_initial);
            ScaleAspect::init(_desc.m_ae);
            RenderAspect::init(_desc.m_ae.m_smoother);
        }
    };

  }  // namespace Engine::Parameters

}  // namespace Engine
