#pragma once

#include "Parameters.h"

namespace nltools
{
  namespace msg
  {

    // todo: deprecate
    namespace Parameters
    {
      // note: hard-coded counts should only be temporary
      static_assert(C15::Parameters::num_of_Macro_Controls == 6);
      static_assert(C15::Parameters::num_of_Hardware_Sources == 10);
      static_assert(C15::Parameters::num_of_Hardware_Amounts
                    == (C15::Parameters::num_of_Macro_Controls * C15::Parameters::num_of_Hardware_Sources));

      struct Parameter
      {
        uint16_t m_id {};
        double m_controlPosition = 0;
      };

      // todo: remove (unused)
      //      struct RibbonParameter : Parameter
      //      {
      //        RibbonTouchBehaviour ribbonTouchBehaviour {};
      //        RibbonReturnMode ribbonReturnMode {};
      //      };

      // todo: remove (unused)
      //      struct PedalParameter : Parameter
      //      {
      //        PedalModes pedalMode {};
      //        ReturnMode returnMode {};
      //      };

      struct MacroParameter : Parameter
      {
      };

      struct ModulateableParameter : Parameter
      {
        MacroControls m_macro = MacroControls::NONE;
        double m_modulationAmount = 0;
      };

      struct UnmodulateableParameter : Parameter
      {
      };

      // todo: remove (unnecessary)
      struct GlobalParameter : Parameter
      {
      };

      struct HardwareSourceParameter : Parameter
      {
        ReturnMode m_returnMode = ReturnMode::None;
      };

      struct HardwareAmountParameter : Parameter
      {
      };

      // todo: remove (unnecessary)
      struct SplitPoint : ModulateableParameter
      {
      };

      inline bool operator==(const Parameter& lhs, const Parameter& rhs)
      {
        auto ret = lhs.m_id == rhs.m_id;
        ret &= lhs.m_controlPosition == rhs.m_controlPosition;
        return ret;
      }

      inline bool operator==(const ModulateableParameter& lhs, const ModulateableParameter& rhs)
      {
        auto ret = lhs.m_id == rhs.m_id;
        ret &= lhs.m_controlPosition == rhs.m_controlPosition;
        ret &= lhs.m_modulationAmount == rhs.m_modulationAmount;
        ret &= lhs.m_macro == rhs.m_macro;
        return ret;
      }

    }  // namespace nltools::msg::Parameters

    // todo: refactor (into Global or Polyphonic/Monophonic Modulateables/Unmodulateables, deprecating ParameterGroups)
    namespace ParameterGroups
    {

      struct UnisonGroup
      {
        Parameters::UnmodulateableParameter unisonVoices;
        Parameters::ModulateableParameter detune;
        Parameters::UnmodulateableParameter phase;
        Parameters::UnmodulateableParameter pan;
      };

      struct MonoGroup
      {
        Parameters::UnmodulateableParameter monoEnable;
        Parameters::UnmodulateableParameter legato;
        Parameters::UnmodulateableParameter priority;
        Parameters::ModulateableParameter glide;
      };

      struct MasterGroup
      {
        Parameters::ModulateableParameter volume;
        Parameters::ModulateableParameter tune;
        Parameters::ModulateableParameter pan;
        Parameters::ModulateableParameter serialFX;
      };

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

    }  // namespace nltools::msg::ParameterGroups

    namespace detail
    {

      // prototype, not to use directly
      template <MessageType M> struct PresetMessage
      {
       protected:
        // nltools requirements - message type safety
        static_assert(M == MessageType::SinglePreset || M == MessageType::SplitPreset || M == MessageType::LayerPreset,
                      "PresetMessage can only be of MessageType (Single/Split/Layer)Preset");
        // provide types
        using ParameterType = C15::Descriptors::ParameterType;
        template <ParameterType P, typename T> using SingularParameterArray = C15::Storage::Array<P, T>;
        template <ParameterType P, typename T>
        using DualParameterArray = std::array<SingularParameterArray<P, T>, C15::Properties::num_of_VoiceGroups>;

       public:
        // nltools requirements - message type retrieval
        constexpr static nltools::msg::MessageType getType()
        {
          return M;
        }

        // shared data (present in any sound type)
        // todo: refactor ...
        SingularParameterArray<ParameterType::Hardware_Source, Parameters::HardwareSourceParameter> hwsources;
        SingularParameterArray<ParameterType::Hardware_Amount, Parameters::HardwareAmountParameter> hwamounts;
        SingularParameterArray<ParameterType::Macro_Control, Parameters::MacroParameter> macros;
        SingularParameterArray<ParameterType::Macro_Time, Parameters::UnmodulateableParameter> macrotimes;
        // ... into:
        SingularParameterArray<ParameterType::Hardware_Source, controls::HardwareSourceParameter> m_hardwareSources;
        SingularParameterArray<ParameterType::Hardware_Amount, controls::HardwareAmountParameter> m_hardwareAmounts;
        SingularParameterArray<ParameterType::Macro_Control, controls::MacroControlParameter> m_macroControls;
        SingularParameterArray<ParameterType::Macro_Time, controls::MacroTimeParameter> m_macroTimes;

        // todo: refactor (into Global Modulateables/Unmodulateables, deprecating ParameterGroups)
        ParameterGroups::MasterGroup master;
        Parameters::GlobalParameter scaleBaseKey;
        std::array<Parameters::ModulateableParameter, 12> scaleOffsets;
        // todo: use
        SingularParameterArray<ParameterType::Global_Modulateable, controls::GlobalModulateableParameter>
            m_globalModulateables;
        SingularParameterArray<ParameterType::Global_Unmodulateable, controls::GlobalUnmodulateableParameter>
            m_globalUnmodulateables;
        DualParameterArray<ParameterType::Monophonic_Modulateable, controls::MonophonicModulateableParameter>
            m_monophonicModulateables;
        DualParameterArray<ParameterType::Monophonic_Unmodulateable, controls::MonophonicUnmodulateableParameter>
            m_monophonicUnmodulateables;

        // comparison
        static inline bool compareCommon(const PresetMessage<M>& _lhs, const PresetMessage<M>& _rhs)
        {
          auto ret = _lhs.m_hardwareSources == _rhs.m_hardwareSources;
          ret &= _lhs.m_hardwareAmounts == _rhs.m_hardwareAmounts;
          ret &= _lhs.m_macroControls == _rhs.m_macroControls;
          ret &= _lhs.m_macroTimes == _rhs.m_macroTimes;
          ret &= _lhs.m_globalModulateables == _rhs.m_globalModulateables;
          ret &= _lhs.m_globalUnmodulateables == _rhs.m_globalUnmodulateables;
          ret &= _lhs.m_monophonicModulateables == _rhs.m_monophonicModulateables;
          ret &= _lhs.m_monophonicUnmodulateables == _rhs.m_monophonicUnmodulateables;
          // todo: remove
          ret = _lhs.hwsources == _rhs.hwsources;
          ret &= _lhs.hwamounts == _rhs.hwamounts;
          ret &= _lhs.macros == _rhs.macros;
          ret &= _lhs.macrotimes == _rhs.macrotimes;
          // todo: remove (when refactored into Global Modulateables/Unmodulateables)
          ret &= _lhs.master == _rhs.master;
          ret &= _lhs.scaleBaseKey == _rhs.scaleBaseKey;
          ret &= _lhs.scaleOffsets == _rhs.scaleOffsets;
          return ret;
        }
      };

    }  // namespace nltools::msg::detail

    struct SinglePresetMessage : public detail::PresetMessage<nltools::msg::MessageType::SinglePreset>
    {
      // todo: refactor (into Polyphonic/Monophonic Modulateables/Unmodulateables)
      std::array<Parameters::ModulateableParameter, 169> modulateables;
      std::array<Parameters::UnmodulateableParameter, 29> unmodulateables;

      // todo: refactor (into Polyphonic Modulateables/Unmodulateables, deprecating ParameterGroups)
      ParameterGroups::UnisonGroup unison;
      ParameterGroups::MonoGroup mono;

      // todo: use
      SingularParameterArray<ParameterType::Polyphonic_Modulateable, controls::PolyphonicModulateableParameter>
          m_polyphonicModulateables;
      SingularParameterArray<ParameterType::Polyphonic_Unmodulateable, controls::PolyphonicUnmodulateableParameter>
          m_polyphonicUnmodulateables;
    };

    inline bool operator==(const SinglePresetMessage& _lhs, const SinglePresetMessage& _rhs)
    {
      auto ret = SinglePresetMessage::compareCommon(_lhs, _rhs);
      ret &= _lhs.m_polyphonicModulateables == _rhs.m_polyphonicModulateables;
      ret &= _lhs.m_polyphonicUnmodulateables == _rhs.m_polyphonicUnmodulateables;
      // todo: remove (when refactored into Polyphonic/Monophonic Modulateables/Unmodulateables)
      ret &= _lhs.unmodulateables == _rhs.unmodulateables;
      ret &= _lhs.modulateables == _rhs.modulateables;
      ret &= _lhs.mono == _rhs.mono;
      ret &= _lhs.unison == _rhs.unison;
      return ret;
    }

    inline bool operator!=(const SinglePresetMessage& _lhs, const SinglePresetMessage& _rhs)
    {
      return !(_lhs == _rhs);
    }

    struct SplitPresetMessage : public detail::PresetMessage<nltools::msg::MessageType::SplitPreset>
    {
      // todo: refactor (into Polyphonic/Monophonic Modulateables/Unmodulateables)
      std::array<std::array<Parameters::ModulateableParameter, 169>, 2> modulateables;
      std::array<std::array<Parameters::UnmodulateableParameter, 29>, 2> unmodulateables;

      // todo: refactor (into Polyphonic Modulateables/Unmodulateables, deprecating ParameterGroups)
      std::array<ParameterGroups::UnisonGroup, 2> unison;
      std::array<ParameterGroups::MonoGroup, 2> mono;
      std::array<Parameters::SplitPoint, 2> splitpoint;

      // todo: use
      DualParameterArray<ParameterType::Polyphonic_Modulateable, controls::PolyphonicModulateableParameter>
          m_polyphonicModulateables;
      DualParameterArray<ParameterType::Polyphonic_Unmodulateable, controls::PolyphonicUnmodulateableParameter>
          m_polyphonicUnmodulateables;
    };

    inline bool operator==(const SplitPresetMessage& _lhs, const SplitPresetMessage& _rhs)
    {
      auto ret = SplitPresetMessage::compareCommon(_lhs, _rhs);
      ret &= _lhs.m_polyphonicModulateables == _rhs.m_polyphonicModulateables;
      ret &= _lhs.m_polyphonicUnmodulateables == _rhs.m_polyphonicUnmodulateables;
      // todo: remove (when refactored into Polyphonic/Monophonic Modulateables/Unmodulateables)
      ret &= _lhs.unmodulateables == _rhs.unmodulateables;
      ret &= _lhs.modulateables == _rhs.modulateables;
      ret &= _lhs.mono == _rhs.mono;
      ret &= _lhs.unison == _rhs.unison;
      ret &= _lhs.splitpoint == _rhs.splitpoint;
      return ret;
    }

    inline bool operator!=(const SplitPresetMessage& _lhs, const SplitPresetMessage& _rhs)
    {
      return !(_lhs == _rhs);
    }

    struct LayerPresetMessage : public detail::PresetMessage<nltools::msg::MessageType::LayerPreset>
    {
      // todo: refactor (into Polyphonic/Monophonic Modulateables/Unmodulateables)
      std::array<std::array<Parameters::ModulateableParameter, 169>, 2> modulateables;
      std::array<std::array<Parameters::UnmodulateableParameter, 29>, 2> unmodulateables;

      // todo: refactor (into Polyphonic Modulateables/Unmodulateables, deprecating ParameterGroups)
      // note: yes, Unison and Mono will be present twice in a LayerPresetMsg (although only one VoiceGroup is relevant)
      //       (for comparisons to work properly, Unison/Mono should be identical in both VoiceGroups)
      ParameterGroups::UnisonGroup unison;
      ParameterGroups::MonoGroup mono;

      // todo: use
      DualParameterArray<ParameterType::Polyphonic_Modulateable, controls::PolyphonicModulateableParameter>
          m_polyphonicModulateables;
      DualParameterArray<ParameterType::Polyphonic_Unmodulateable, controls::PolyphonicUnmodulateableParameter>
          m_polyphonicUnmodulateables;
    };

    inline bool operator==(const LayerPresetMessage& _lhs, const LayerPresetMessage& _rhs)
    {
      auto ret = LayerPresetMessage::compareCommon(_lhs, _rhs);
      ret &= _lhs.m_polyphonicModulateables == _rhs.m_polyphonicModulateables;
      ret &= _lhs.m_polyphonicUnmodulateables == _rhs.m_polyphonicUnmodulateables;
      // todo: remove (when refactored into Polyphonic/Monophonic Modulateables/Unmodulateables)
      ret &= _lhs.unmodulateables == _rhs.unmodulateables;
      ret &= _lhs.modulateables == _rhs.modulateables;
      ret &= _lhs.mono == _rhs.mono;
      ret &= _lhs.unison == _rhs.unison;
      return ret;
    }

    inline bool operator!=(const LayerPresetMessage& _lhs, const LayerPresetMessage& _rhs)
    {
      return !(_lhs == _rhs);
    }

  }  // namespace nltools::msg

}  // namespace nltools
