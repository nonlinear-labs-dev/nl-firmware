#pragma once

#include "Parameters.h"

namespace nltools
{
  namespace msg
  {

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
        SingularParameterArray<ParameterType::Hardware_Source, controls::HardwareSourceParameter> m_hardwareSources;
        SingularParameterArray<ParameterType::Hardware_Amount, controls::HardwareAmountParameter> m_hardwareAmounts;
        SingularParameterArray<ParameterType::Macro_Control, controls::MacroControlParameter> m_macroControls;
        SingularParameterArray<ParameterType::Macro_Time, controls::MacroTimeParameter> m_macroTimes;
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
      // data
      SingularParameterArray<ParameterType::Polyphonic_Modulateable, controls::PolyphonicModulateableParameter>
          m_polyphonicModulateables;
      SingularParameterArray<ParameterType::Polyphonic_Unmodulateable, controls::PolyphonicUnmodulateableParameter>
          m_polyphonicUnmodulateables;

      // validation
      static inline bool validate(const SinglePresetMessage& _msg)
      {
        if(!SinglePresetMessage::validateCommon(_msg))
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
      // data
      DualParameterArray<ParameterType::Polyphonic_Modulateable, controls::PolyphonicModulateableParameter>
          m_polyphonicModulateables;
      DualParameterArray<ParameterType::Polyphonic_Unmodulateable, controls::PolyphonicUnmodulateableParameter>
          m_polyphonicUnmodulateables;

      // validation
      static inline bool validate(const SplitPresetMessage& _msg)
      {
        if(!SplitPresetMessage::validateCommon(_msg))
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
      // data
      DualParameterArray<ParameterType::Polyphonic_Modulateable, controls::PolyphonicModulateableParameter>
          m_polyphonicModulateables;
      DualParameterArray<ParameterType::Polyphonic_Unmodulateable, controls::PolyphonicUnmodulateableParameter>
          m_polyphonicUnmodulateables;

      // validation
      static inline bool validate(const LayerPresetMessage& _msg)
      {
        if(!LayerPresetMessage::validateCommon(_msg))
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
          const auto index = (uint32_t) id;
          // copy data from Part I to Part II (assuming already correct id)
          auto& lhs = m_polyphonicModulateables[0][index];
          const auto& rhs = m_polyphonicModulateables[1][index];
          lhs.m_controlPosition = rhs.m_controlPosition;
          lhs.m_macro = rhs.m_macro;
          lhs.m_modulationAmount = rhs.m_modulationAmount;
        }
      }

     private:
      static constexpr C15::Parameters::Polyphonic_Modulateables s_voiceParams[] = {
        C15::Parameters::Polyphonic_Modulateables::Unison_Detune,
        C15::Parameters::Polyphonic_Modulateables::Unison_Phase,
        C15::Parameters::Polyphonic_Modulateables::Unison_Pan,
        C15::Parameters::Polyphonic_Modulateables::Mono_Grp_Glide,
      };
    };

    inline bool operator==(const LayerPresetMessage& _lhs, const LayerPresetMessage& _rhs)
    {
      auto ret = LayerPresetMessage::compareCommon(_lhs, _rhs);
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
