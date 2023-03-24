#pragma once

#include <Parameters.h>
#include <parameter_group.h>
#include <setting_list.h>
#include <SplitPointRounding.h>

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
        inline void init(const float &_initial)
        {
          m_initial = _initial;
          m_position = _initial;
        }
        inline bool update_position(const float &_position)
        {
          if(_position != m_position)
          {
            m_position = _position;
            return true;
          }
          return false;
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
        struct Scaling
        {
          C15::Properties::SmootherScale m_scaleId;
          float m_scaleFactor = {}, m_scaleOffset = {};
        } m_scaling = {};
        float m_scaled = {};
        inline void init(const C15::Descriptors::AudioEngineParamDescriptor &_desc)
        {
          m_scaling.m_scaleId = _desc.m_scaleId;
          m_scaling.m_scaleFactor = _desc.m_scaleFactor;
          m_scaling.m_scaleOffset = _desc.m_scaleOffset;
        }
      };

      struct TimeAspect
      {
        struct Time
        {
          float m_dxAudio = {}, m_dxFast = {}, m_dxSlow = {};
        } m_time = {};
      };

      struct RenderAspect
      {
        struct Rendering
        {
          C15::Descriptors::SmootherSection m_renderSection;
          C15::Descriptors::SmootherClock m_renderClock;
          uint32_t m_renderIndex = {};
        } m_rendering = {};
        inline void init(const C15::Descriptors::SmootherDescriptor &_desc)
        {
          m_rendering.m_renderSection = _desc.m_section;
          m_rendering.m_renderClock = _desc.m_clock;
          m_rendering.m_renderIndex = _desc.m_index;
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
      inline void log(const char *const _msg, const C15::ParameterDescriptor &_descriptor)
      {
        nltools::Log::info(_msg, "(label:", C15::ParameterGroups[(unsigned) _descriptor.m_group].m_label_short,
                           _descriptor.m_pg.m_param_label_long, ", index:", _descriptor.m_param.m_index,
                           ", position:", m_position, ", behavior: ", (int) m_behavior, ")");
      }
    };

    struct HardwareAmount : public Aspects::ParameterAspect
    {
      uint32_t m_sourceId = {};
      inline void log(const char *const _msg, const C15::ParameterDescriptor &_descriptor)
      {
        nltools::Log::info(_msg, "(label:", C15::ParameterGroups[(unsigned) _descriptor.m_group].m_label_short,
                           _descriptor.m_pg.m_param_label_long, ", index:", _descriptor.m_param.m_index,
                           ", position:", m_position, ")");
      }
    };

    struct MacroControl : public Aspects::ParameterAspect, public Aspects::RangeAspect
    {
      uint32_t m_index = {};
      inline void init(const C15::ParameterDescriptor &_desc)
      {
        ParameterAspect::init(_desc.m_initial);
        m_index = _desc.m_param.m_index;
      }
      inline void log(const char *const _msg, const C15::ParameterDescriptor &_descriptor)
      {
        nltools::Log::info(_msg, "(label:", C15::ParameterGroups[(unsigned) _descriptor.m_group].m_label_short,
                           _descriptor.m_pg.m_param_label_long, ", index:", _descriptor.m_param.m_index,
                           ", position:", m_position, ")");
      }
    };

    struct ScaledParameter : public Aspects::ParameterAspect, public Aspects::ScaleAspect
    {
      inline void init(const C15::Properties::SmootherScale &_id, const float &_factor, const float &_offset,
                       const float &_value)
      {
        ParameterAspect::init(_value);
        m_scaling.m_scaleId = _id;
        m_scaling.m_scaleFactor = _factor;
        m_scaling.m_scaleOffset = _offset;
      }
      inline void init(const C15::Settings::SettingDescriptor &_desc)
      {
        const float initial_value
            = _desc.m_default_value.has_value() ? std::get<float>(_desc.m_default_value.value()) : 0.0f;
        init(_desc.m_render_scaling.m_scaleId, _desc.m_render_scaling.m_scaleFactor,
             _desc.m_render_scaling.m_scaleOffset, initial_value);
      }
    };

    struct TimeParameter : public ScaledParameter, public Aspects::TimeAspect
    {
    };

    struct MacroTime : public Aspects::ParameterAspect, public Aspects::ScaleAspect, public Aspects::TimeAspect
    {
      inline void init(const C15::ParameterDescriptor &_desc)
      {
        ParameterAspect::init(_desc.m_initial);
        ScaleAspect::init(_desc.m_ae);
      }
      inline void log(const char *const _msg, const C15::ParameterDescriptor &_descriptor)
      {
        nltools::Log::info(_msg, "(label:", C15::ParameterGroups[(unsigned) _descriptor.m_group].m_label_short,
                           _descriptor.m_pg.m_param_label_long, ", index:", _descriptor.m_param.m_index,
                           ", position:", m_position, ", scaled:", m_scaled, ")");
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
        if(m_splitpoint)
        {
          m_unclipped = m_base + SplitPointRounding::getModulation(_mod, m_amount);
        }
        else
        {
          m_unclipped = m_base + (m_amount * _mod);
        }
        return m_position != m_unclipped;  // basic change detection (position needs to be updated from outside)
      }
      inline void update_modulation_aspects(const float &_mod)
      {
        if(m_splitpoint)
        {
          m_base = m_position - SplitPointRounding::getModulation(_mod, m_amount);
        }
        else
        {
          m_base = m_position - (m_amount * _mod);
        }
        m_ceil = m_base + m_amount;
      }
      inline void log(const char *const _msg, const C15::ParameterDescriptor &_descriptor)
      {
        nltools::Log::info(_msg, "(label:", C15::ParameterGroups[(unsigned) _descriptor.m_group].m_label_short,
                           _descriptor.m_pg.m_param_label_long, ", index:", _descriptor.m_param.m_index,
                           ", position:", m_position, ", mc:", (int) m_source, ", amt:", m_amount,
                           ", scaled:", m_scaled, ")");
      }
      inline void log(const uint32_t &_layer, const char *const _msg, const C15::ParameterDescriptor &_descriptor)
      {
        nltools::Log::info(
            _msg, "(layer: ", _layer, ", label:", C15::ParameterGroups[(unsigned) _descriptor.m_group].m_label_short,
            _descriptor.m_pg.m_param_label_long, ", index:", _descriptor.m_param.m_index, ", position:", m_position,
            ", mc:", (int) m_source, ", amt:", m_amount, ", scaled:", m_scaled, ")");
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
      inline void log(const char *const _msg, const C15::ParameterDescriptor &_descriptor)
      {
        nltools::Log::info(_msg, "(label:", C15::ParameterGroups[(unsigned) _descriptor.m_group].m_label_short,
                           _descriptor.m_pg.m_param_label_long, ", index:", _descriptor.m_param.m_index,
                           ", position:", m_position, ", scaled:", m_scaled, ")");
      }
      inline void log(const uint32_t &_layer, const char *const _msg, const C15::ParameterDescriptor &_descriptor)
      {
        nltools::Log::info(_msg, "(layer: ", _layer,
                           ", label:", C15::ParameterGroups[(unsigned) _descriptor.m_group].m_label_short,
                           _descriptor.m_pg.m_param_label_long, ", index:", _descriptor.m_param.m_index,
                           ", position:", m_position, ", scaled:", m_scaled, ")");
      }
    };

  }  // namespace Engine::Parameters

}  // namespace Engine
