#pragma once

#include "Parameters.h"

namespace nltools
{
  namespace msg
  {

    // todo: remove
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
        SingularParameterArray<ParameterType::Hardware_Source, Parameters::HardwareSourceParameter> hwsources;
        SingularParameterArray<ParameterType::Hardware_Amount, Parameters::HardwareAmountParameter> hwamounts;
        SingularParameterArray<ParameterType::Macro_Control, Parameters::MacroParameter> macros;
        SingularParameterArray<ParameterType::Macro_Time, Parameters::UnmodulateableParameter> macrotimes;
        // ... into:
        SingularParameterArray<ParameterType::Hardware_Source, controls::HardwareSourceParameter> m_hardwareSources;
        SingularParameterArray<ParameterType::Hardware_Amount, controls::HardwareAmountParameter> m_hardwareAmounts;
        SingularParameterArray<ParameterType::Macro_Control, controls::MacroControlParameter> m_macroControls;
        SingularParameterArray<ParameterType::Macro_Time, controls::MacroTimeParameter> m_macroTimes;

        //todo: remove:
        ParameterGroups::MasterGroup master;
        Parameters::GlobalParameter scaleBaseKey;
        std::array<Parameters::ModulateableParameter, 12> scaleOffsets;

        // done: use
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
          return ret;
        }

        // validation
        static inline bool validateCommon(const PresetMessage<M>& _msg)
        {
          for(const auto& element : _msg.m_hardwareSources)
            if(!element.validateParameterType())
              return false;
          for(const auto& element : _msg.m_hardwareAmounts)
            if(!element.validateParameterType())
              return false;
          for(const auto& element : _msg.m_macroControls)
            if(!element.validateParameterType())
              return false;
          for(const auto& element : _msg.m_macroTimes)
            if(!element.validateParameterType())
              return false;
          for(const auto& element : _msg.m_globalModulateables)
            if(!element.validateParameterType())
              return false;
          for(const auto& element : _msg.m_globalUnmodulateables)
            if(!element.validateParameterType())
              return false;
          for(const auto& layer : _msg.m_monophonicModulateables)
            for(const auto& element : layer)
              if(!element.validateParameterType())
                return false;
          for(const auto& layer : _msg.m_monophonicUnmodulateables)
            for(const auto& element : layer)
              if(!element.validateParameterType())
                return false;
          return true;
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

      // temporary: local parameters
      SingularParameterArray<ParameterType::Local_Modulateable, controls::LocalModulateableParameter> m_localModulateables;
      SingularParameterArray<ParameterType::Local_Unmodulateable, controls::LocalUnmodulateableParameter> m_localUnmodulateables;

      // todo: use
      SingularParameterArray<ParameterType::Polyphonic_Modulateable, controls::PolyphonicModulateableParameter>
          m_polyphonicModulateables;
      SingularParameterArray<ParameterType::Polyphonic_Unmodulateable, controls::PolyphonicUnmodulateableParameter>
          m_polyphonicUnmodulateables;

      // validation
      static inline bool validate(const SinglePresetMessage& _msg)
      {
        if(!SinglePresetMessage::validateCommon(_msg))
          return false;
        // temporary: local parameters
          for(const auto& element : _msg.m_localUnmodulateables)
            if(!element.validateParameterType())
              return false;
          for(const auto& element : _msg.m_localUnmodulateables)
            if(!element.validateParameterType())
              return false;
        // polyphonic parameters
          for(const auto& element : _msg.m_polyphonicUnmodulateables)
            if(!element.validateParameterType())
              return false;
          for(const auto& element : _msg.m_polyphonicUnmodulateables)
            if(!element.validateParameterType())
              return false;
          return true;
      }
    };

    inline bool operator==(const SinglePresetMessage& _lhs, const SinglePresetMessage& _rhs)
    {
      auto ret = SinglePresetMessage::compareCommon(_lhs, _rhs);
      // temporary: local parameters
      ret &= _lhs.m_localModulateables == _rhs.m_localModulateables;
      ret &= _lhs.m_localUnmodulateables == _rhs.m_localUnmodulateables;
      // polyphonic parameters
      ret &= _lhs.m_polyphonicModulateables == _rhs.m_polyphonicModulateables;
      ret &= _lhs.m_polyphonicUnmodulateables == _rhs.m_polyphonicUnmodulateables;
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

      // temporary: local parameters
      DualParameterArray<ParameterType::Local_Modulateable, controls::LocalModulateableParameter> m_localModulateables;
      DualParameterArray<ParameterType::Local_Unmodulateable, controls::LocalUnmodulateableParameter> m_localUnmodulateables;

      // todo: use
      DualParameterArray<ParameterType::Polyphonic_Modulateable, controls::PolyphonicModulateableParameter>
          m_polyphonicModulateables;
      DualParameterArray<ParameterType::Polyphonic_Unmodulateable, controls::PolyphonicUnmodulateableParameter>
          m_polyphonicUnmodulateables;

      // validation
      static inline bool validate(const SplitPresetMessage& _msg)
      {
        if(!SplitPresetMessage::validateCommon(_msg))
          return false;
        // temporary: local parameters
        for(const auto& layer : _msg.m_localUnmodulateables)
          for(const auto& element : layer)
            if(!element.validateParameterType())
              return false;
        for(const auto& layer : _msg.m_localModulateables)
          for(const auto& element : layer)
            if(!element.validateParameterType())
              return false;
        // polyphonic parameters
        for(const auto& layer : _msg.m_polyphonicModulateables)
          for(const auto& element : layer)
            if(!element.validateParameterType())
              return false;
        for(const auto& layer : _msg.m_polyphonicUnmodulateables)
          for(const auto& element : layer)
            if(!element.validateParameterType())
              return false;
        return true;
      }
    };

    inline bool operator==(const SplitPresetMessage& _lhs, const SplitPresetMessage& _rhs)
    {
      auto ret = SplitPresetMessage::compareCommon(_lhs, _rhs);
      // temporary: local parameters
      ret &= _lhs.m_localModulateables == _rhs.m_localModulateables;
      ret &= _lhs.m_localUnmodulateables == _rhs.m_localUnmodulateables;
      // polyphonic parameters
      ret &= _lhs.m_polyphonicModulateables == _rhs.m_polyphonicModulateables;
      ret &= _lhs.m_polyphonicUnmodulateables == _rhs.m_polyphonicUnmodulateables;
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

      // temporary: local parameters
      DualParameterArray<ParameterType::Local_Modulateable, controls::LocalModulateableParameter> m_localModulateables;
      DualParameterArray<ParameterType::Local_Unmodulateable, controls::LocalUnmodulateableParameter> m_localUnmodulateables;

      // todo: use
      DualParameterArray<ParameterType::Polyphonic_Modulateable, controls::PolyphonicModulateableParameter>
          m_polyphonicModulateables;
      DualParameterArray<ParameterType::Polyphonic_Unmodulateable, controls::PolyphonicUnmodulateableParameter>
          m_polyphonicUnmodulateables;

      // validation
      static inline bool validate(const LayerPresetMessage& _msg)
      {
        if(!LayerPresetMessage::validateCommon(_msg))
          return false;
        // temporary: local parameters
        for(const auto& layer : _msg.m_localUnmodulateables)
          for(const auto& element : layer)
            if(!element.validateParameterType())
              return false;
        for(const auto& layer : _msg.m_localModulateables)
          for(const auto& element : layer)
            if(!element.validateParameterType())
              return false;
        // polyphonic parameters
        for(const auto& layer : _msg.m_polyphonicModulateables)
          for(const auto& element : layer)
            if(!element.validateParameterType())
              return false;
        for(const auto& layer : _msg.m_polyphonicUnmodulateables)
          for(const auto& element : layer)
            if(!element.validateParameterType())
              return false;
        return true;
      }

      // use this function to ensure that smoothed parameters of the "Voices" supergroup are identical in both Parts
      inline void guaranteeEqualVoicesParameters()
      {
          for(const auto id : s_voiceParams)
          {
              const auto index = (uint32_t)id;
              // copy data from Part I to Part II (assuming already correct id)
              auto &lhs = m_localModulateables[0][index];
              const auto &rhs = m_localModulateables[1][index];
              lhs.m_controlPosition = rhs.m_controlPosition;
              lhs.m_macro = rhs.m_macro;
              lhs.m_modulationAmount = rhs.m_modulationAmount;
          }
      }
    private:
      static constexpr C15::Parameters::Local_Modulateables s_voiceParams[] = {
          C15::Parameters::Local_Modulateables::Unison_Detune,
          C15::Parameters::Local_Modulateables::Unison_Phase,
          C15::Parameters::Local_Modulateables::Unison_Pan,
          C15::Parameters::Local_Modulateables::Mono_Grp_Glide,
      };
    };

    inline bool operator==(const LayerPresetMessage& _lhs, const LayerPresetMessage& _rhs)
    {
      auto ret = LayerPresetMessage::compareCommon(_lhs, _rhs);
      // temporary: local parameters
      ret &= _lhs.m_localModulateables == _rhs.m_localModulateables;
      ret &= _lhs.m_localUnmodulateables == _rhs.m_localUnmodulateables;
      // polyphonic parameters
      ret &= _lhs.m_polyphonicModulateables == _rhs.m_polyphonicModulateables;
      ret &= _lhs.m_polyphonicUnmodulateables == _rhs.m_polyphonicUnmodulateables;
      return ret;
    }

    inline bool operator!=(const LayerPresetMessage& _lhs, const LayerPresetMessage& _rhs)
    {
      return !(_lhs == _rhs);
    }

  }  // namespace nltools::msg

}  // namespace nltools
