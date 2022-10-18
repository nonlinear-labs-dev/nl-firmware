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
      constexpr auto numMacros = static_cast<size_t>(C15::Parameters::Macro_Controls::_LENGTH_) - 1; //not cool!
      static_assert(numMacros == 6);
      constexpr auto numHWSources = static_cast<size_t>(C15::Parameters::Hardware_Sources::_LENGTH_); //cool!
      static_assert(numHWSources == 10);
      constexpr auto numRouters = static_cast<size_t>(C15::Parameters::Hardware_Amounts::_LENGTH_);
      static_assert(numRouters == (numMacros * numHWSources));

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

    struct SinglePresetMessage
    {
      constexpr static nltools::msg::MessageType getType()
      {
        return nltools::msg::MessageType::SinglePreset;
      }

      std::array<ParameterGroups::MacroParameter, ParameterGroups::numMacros> macros;
      std::array<ParameterGroups::UnmodulateableParameter, ParameterGroups::numMacros> macrotimes;

      std::array<ParameterGroups::ModulateableParameter, 169> modulateables;
      std::array<ParameterGroups::UnmodulateableParameter, 29> unmodulateables;

      std::array<ParameterGroups::HardwareSourceParameter, ParameterGroups::numHWSources> hwsources;
      std::array<ParameterGroups::HardwareAmountParameter, ParameterGroups::numRouters> hwamounts;

      ParameterGroups::UnisonGroup unison;
      ParameterGroups::MonoGroup mono;

      ParameterGroups::MasterGroup master;

      ParameterGroups::GlobalParameter scaleBaseKey;
      std::array<ParameterGroups::ModulateableParameter, 12> scaleOffsets;
    };

    inline bool operator==(const SinglePresetMessage& lhs, const SinglePresetMessage& rhs)
    {
      auto ret = lhs.unmodulateables == rhs.unmodulateables;
      ret &= lhs.modulateables == rhs.modulateables;
      ret &= lhs.scaleOffsets == rhs.scaleOffsets;
      ret &= lhs.scaleBaseKey == rhs.scaleBaseKey;
      ret &= lhs.mono == rhs.mono;
      ret &= lhs.unison == rhs.unison;
      ret &= lhs.master == rhs.master;
      ret &= lhs.hwamounts == rhs.hwamounts;
      ret &= lhs.hwsources == rhs.hwsources;
      ret &= lhs.macros == rhs.macros;
      ret &= lhs.macrotimes == rhs.macrotimes;
      return ret;
    }

    inline bool operator!=(const SinglePresetMessage& lhs, const SinglePresetMessage& rhs)
    {
      return !(lhs == rhs);
    }

    struct SplitPresetMessage
    {
      constexpr static nltools::msg::MessageType getType()
      {
        return nltools::msg::MessageType::SplitPreset;
      }

      std::array<std::array<ParameterGroups::ModulateableParameter, 169>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulateableParameter, 29>, 2> unmodulateables;

      std::array<ParameterGroups::UnisonGroup, 2> unison;
      std::array<ParameterGroups::MonoGroup, 2> mono;

      ParameterGroups::MasterGroup master;

      std::array<ParameterGroups::HardwareSourceParameter, ParameterGroups::numHWSources> hwsources;
      std::array<ParameterGroups::HardwareAmountParameter, ParameterGroups::numRouters> hwamounts;

      std::array<ParameterGroups::MacroParameter, ParameterGroups::numMacros> macros;
      std::array<ParameterGroups::UnmodulateableParameter, ParameterGroups::numMacros> macrotimes;

      ParameterGroups::GlobalParameter scaleBaseKey;
      std::array<ParameterGroups::ModulateableParameter, 12> scaleOffsets;
      std::array<ParameterGroups::SplitPoint, 2> splitpoint;
    };

    inline bool operator==(const SplitPresetMessage& lhs, const SplitPresetMessage& rhs)
    {
      auto ret = lhs.unmodulateables == rhs.unmodulateables;
      ret &= lhs.modulateables == rhs.modulateables;
      ret &= lhs.scaleOffsets == rhs.scaleOffsets;
      ret &= lhs.scaleBaseKey == rhs.scaleBaseKey;
      ret &= lhs.mono == rhs.mono;
      ret &= lhs.unison == rhs.unison;
      ret &= lhs.master == rhs.master;
      ret &= lhs.hwamounts == rhs.hwamounts;
      ret &= lhs.hwsources == rhs.hwsources;
      ret &= lhs.macros == rhs.macros;
      ret &= lhs.macrotimes == rhs.macrotimes;
      ret &= lhs.splitpoint == rhs.splitpoint;
      return ret;
    }

    inline bool operator!=(const SplitPresetMessage& lhs, const SplitPresetMessage& rhs)
    {
      return !(lhs == rhs);
    }

    struct LayerPresetMessage
    {
      constexpr static nltools::msg::MessageType getType()
      {
        return nltools::msg::MessageType::LayerPreset;
      }
      std::array<ParameterGroups::HardwareSourceParameter, ParameterGroups::numHWSources> hwsources;
      std::array<ParameterGroups::HardwareAmountParameter, ParameterGroups::numRouters> hwamounts;

      std::array<ParameterGroups::MacroParameter, ParameterGroups::numMacros> macros;
      std::array<ParameterGroups::UnmodulateableParameter, ParameterGroups::numMacros> macrotimes;

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
      auto ret = lhs.unmodulateables == rhs.unmodulateables;
      ret &= lhs.modulateables == rhs.modulateables;
      ret &= lhs.scaleOffsets == rhs.scaleOffsets;
      ret &= lhs.scaleBaseKey == rhs.scaleBaseKey;
      ret &= lhs.mono == rhs.mono;
      ret &= lhs.unison == rhs.unison;
      ret &= lhs.master == rhs.master;
      ret &= lhs.hwamounts == rhs.hwamounts;
      ret &= lhs.hwsources == rhs.hwsources;
      ret &= lhs.macros == rhs.macros;
      ret &= lhs.macrotimes == rhs.macrotimes;
      return ret;
    }

    inline bool operator!=(const LayerPresetMessage& lhs, const LayerPresetMessage& rhs)
    {
      return !(lhs == rhs);
    }
  }
}