#pragma once
#include <parameter_list.h>
#include <parameter_descriptor.h>
#include <parameter_declarations.h>
#include <nltools/messaging/Message.h>

namespace nltools
{
  namespace msg
  {
    namespace ParameterGroups
    {
      static_assert(C15::Parameters::num_of_Macro_Controls == 6); // note: hard-coded counts should only be temporary
      static_assert(C15::Parameters::num_of_Hardware_Sources == 10);
      static_assert(C15::Parameters::num_of_Hardware_Amounts
                    == (C15::Parameters::num_of_Macro_Controls * C15::Parameters::num_of_Hardware_Sources));

      struct Parameter
      {
        uint16_t id {};
        double controlPosition = 0;
      };

      struct RibbonParameter : Parameter
      {
        RibbonTouchBehaviour ribbonTouchBehaviour {};
        RibbonReturnMode ribbonReturnMode {};
      };

      struct PedalParameter : Parameter
      {
        PedalModes pedalMode {};
        ReturnMode returnMode {};
      };

      struct MacroParameter : Parameter
      {
      };

      struct ModulateableParameter : Parameter
      {
        MacroControls mc = MacroControls::NONE;
        double modulationAmount = 0;
      };

      struct UnmodulateableParameter : Parameter
      {
      };

      struct GlobalParameter : Parameter
      {
      };

      struct HardwareSourceParameter : Parameter
      {
        ReturnMode returnMode = ReturnMode::None;
      };

      struct HardwareAmountParameter : Parameter
      {
      };

      struct SplitPoint : ModulateableParameter
      {
      };

      struct UnisonGroup
      {
        ParameterGroups::UnmodulateableParameter unisonVoices;
        ParameterGroups::ModulateableParameter detune;
        ParameterGroups::UnmodulateableParameter phase;
        ParameterGroups::UnmodulateableParameter pan;
      };

      struct MonoGroup
      {
        ParameterGroups::UnmodulateableParameter monoEnable;
        ParameterGroups::UnmodulateableParameter legato;
        ParameterGroups::UnmodulateableParameter priority;
        ParameterGroups::ModulateableParameter glide;
      };

      struct MasterGroup
      {
        ParameterGroups::ModulateableParameter volume;
        ParameterGroups::ModulateableParameter tune;
        ParameterGroups::ModulateableParameter pan;
        ParameterGroups::ModulateableParameter serialFX;
      };

      inline bool operator==(const Parameter& lhs, const Parameter& rhs)
      {
        auto ret = lhs.id == rhs.id;
        ret &= lhs.controlPosition == rhs.controlPosition;
        return ret;
      }

      inline bool operator==(const ModulateableParameter& lhs, const ModulateableParameter& rhs)
      {
        auto ret = lhs.id == rhs.id;
        ret &= lhs.controlPosition == rhs.controlPosition;
        ret &= lhs.modulationAmount == rhs.modulationAmount;
        ret &= lhs.mc == rhs.mc;
        return ret;
      }

      inline bool operator==(const MonoGroup& lhs, const MonoGroup& rhs)
      {
        auto ret = lhs.glide == rhs.glide;
        ret &= lhs.monoEnable == rhs.monoEnable;
        ret &= lhs.priority == rhs.priority;
        ret &= lhs.legato == rhs.legato;
        return ret;
      }

      inline bool operator==(const UnisonGroup& lhs, const UnisonGroup& rhs)
      {
        auto ret = lhs.unisonVoices == rhs.unisonVoices;
        ret &= lhs.detune == rhs.detune;
        ret &= lhs.pan == rhs.pan;
        ret &= lhs.phase == rhs.phase;
        return ret;
      }

      inline bool operator==(const MasterGroup& lhs, const MasterGroup& rhs)
      {
        auto ret = lhs.volume == rhs.volume;
        ret &= lhs.tune == rhs.tune;
        ret &= lhs.pan == rhs.pan;
        ret &= lhs.serialFX == rhs.serialFX;
        return ret;
      }
    }

    template<nltools::msg::MessageType M>
    struct PresetMessage
    {
        // nltools requirements
        static_assert(
                M == nltools::msg::MessageType::SinglePreset
             || M == nltools::msg::MessageType::SplitPreset
             || M == nltools::msg::MessageType::LayerPreset,
                "PresetMessage can only be of MessageType (Single/Split/Layer)Preset"
        );
        constexpr static nltools::msg::MessageType getType()
        {
          return M;
        }

        // provide types
        using ParameterType = C15::Descriptors::ParameterType;
        template<ParameterType P, typename T>
        using ParameterArray = C15::Storage::Array<P, T>;

        // shared data (present in any sound type)
        ParameterArray<ParameterType::Hardware_Source, ParameterGroups::HardwareSourceParameter> hwsources;
        ParameterArray<ParameterType::Hardware_Amount, ParameterGroups::HardwareAmountParameter> hwamounts;
        ParameterArray<ParameterType::Macro_Control, ParameterGroups::MacroParameter> macros;
        ParameterArray<ParameterType::Macro_Time, ParameterGroups::UnmodulateableParameter> macrotimes;

        // comparison
        static bool compareCommon(const PresetMessage<M>& _lhs, const PresetMessage<M>& _rhs)
        {
            auto ret = _lhs.hwsources == _rhs.hwsources;
            ret &= _lhs.hwamounts == _rhs.hwamounts;
            ret &= _lhs.macros == _rhs.macros;
            ret &= _lhs.macrotimes == _rhs.macrotimes;
            return ret;
        }
    };

    struct SinglePresetMessage : public PresetMessage<nltools::msg::MessageType::SinglePreset>
    {
      std::array<ParameterGroups::ModulateableParameter, 169> modulateables;
      std::array<ParameterGroups::UnmodulateableParameter, 29> unmodulateables;

      ParameterGroups::UnisonGroup unison;
      ParameterGroups::MonoGroup mono;

      ParameterGroups::MasterGroup master;

      ParameterGroups::GlobalParameter scaleBaseKey;
      std::array<ParameterGroups::ModulateableParameter, 12> scaleOffsets;
    };

    inline bool operator==(const SinglePresetMessage& lhs, const SinglePresetMessage& rhs)
    {
      auto ret = SinglePresetMessage::compareCommon(lhs, rhs);
      ret &= lhs.unmodulateables == rhs.unmodulateables;
      ret &= lhs.modulateables == rhs.modulateables;
      ret &= lhs.scaleOffsets == rhs.scaleOffsets;
      ret &= lhs.scaleBaseKey == rhs.scaleBaseKey;
      ret &= lhs.mono == rhs.mono;
      ret &= lhs.unison == rhs.unison;
      ret &= lhs.master == rhs.master;
      return ret;
    }

    inline bool operator!=(const SinglePresetMessage& lhs, const SinglePresetMessage& rhs)
    {
      return !(lhs == rhs);
    }

    struct SplitPresetMessage : public PresetMessage<nltools::msg::MessageType::SplitPreset>
    {

      std::array<std::array<ParameterGroups::ModulateableParameter, 169>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulateableParameter, 29>, 2> unmodulateables;

      std::array<ParameterGroups::UnisonGroup, 2> unison;
      std::array<ParameterGroups::MonoGroup, 2> mono;

      ParameterGroups::MasterGroup master;

      ParameterGroups::GlobalParameter scaleBaseKey;
      std::array<ParameterGroups::ModulateableParameter, 12> scaleOffsets;
      std::array<ParameterGroups::SplitPoint, 2> splitpoint;
    };

    inline bool operator==(const SplitPresetMessage& lhs, const SplitPresetMessage& rhs)
    {
      auto ret = SplitPresetMessage::compareCommon(lhs, rhs);
      ret &= lhs.unmodulateables == rhs.unmodulateables;
      ret &= lhs.modulateables == rhs.modulateables;
      ret &= lhs.scaleOffsets == rhs.scaleOffsets;
      ret &= lhs.scaleBaseKey == rhs.scaleBaseKey;
      ret &= lhs.mono == rhs.mono;
      ret &= lhs.unison == rhs.unison;
      ret &= lhs.master == rhs.master;
      ret &= lhs.splitpoint == rhs.splitpoint;
      return ret;
    }

    inline bool operator!=(const SplitPresetMessage& lhs, const SplitPresetMessage& rhs)
    {
      return !(lhs == rhs);
    }

    struct LayerPresetMessage : public PresetMessage<nltools::msg::MessageType::LayerPreset>
    {

      std::array<std::array<ParameterGroups::ModulateableParameter, 169>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulateableParameter, 29>, 2> unmodulateables;

      ParameterGroups::UnisonGroup unison;
      ParameterGroups::MonoGroup mono;

      ParameterGroups::MasterGroup master;

      ParameterGroups::GlobalParameter scaleBaseKey;
      std::array<ParameterGroups::ModulateableParameter, 12> scaleOffsets;
    };

    inline bool operator==(const LayerPresetMessage& lhs, const LayerPresetMessage& rhs)
    {
      auto ret = LayerPresetMessage::compareCommon(lhs, rhs);
      ret &= lhs.unmodulateables == rhs.unmodulateables;
      ret &= lhs.modulateables == rhs.modulateables;
      ret &= lhs.scaleOffsets == rhs.scaleOffsets;
      ret &= lhs.scaleBaseKey == rhs.scaleBaseKey;
      ret &= lhs.mono == rhs.mono;
      ret &= lhs.unison == rhs.unison;
      ret &= lhs.master == rhs.master;
      return ret;
    }

    inline bool operator!=(const LayerPresetMessage& lhs, const LayerPresetMessage& rhs)
    {
      return !(lhs == rhs);
    }
  }
}
