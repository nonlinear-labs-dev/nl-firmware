#pragma once

#include "Parameters.h"

namespace nltools
{

  namespace msg
  {

    namespace detail
    {

      // generic structures

      template <MessageType M> struct ParameterChangedMessage
      {
        static_assert(M == MessageType::HardwareSourceParameterChanged
                          || M == MessageType::HardwareSourceSendParameterChanged
                          || M == MessageType::HardwareAmountParameterChanged
                          || M == MessageType::MacroControlParameterChanged
                          || M == MessageType::MacroTimeParameterChanged
                          || M == MessageType::GlobalModulateableParameterChanged
                          || M == MessageType::GlobalUnmodulateableParameterChanged
                          || M == MessageType::PolyphonicModulateableParameterChanged
                          || M == MessageType::PolyphonicUnmodulateableParameterChanged
                          || M == MessageType::MonophonicModulateableParameterChanged
                          || M == MessageType::MonophonicUnmodulateableParameterChanged,
                      "MessageType must fit category ParameterChanged");
        constexpr static MessageType getType()
        {
          return M;
        }
      };

      // note: redundant (implemented but unused)
      struct ModulateableChangedMessage
      {
        controls::Position m_modulationUpper = {};
        controls::Position m_modulationLower = {};
      };

      struct LocalChangedMessage
      {
        VoiceGroup m_voiceGroup = VoiceGroup::Invalid;
      };

    }  // namespace nltools::msg::detail

    // usable messages

    struct HardwareSourceParameterChangedMessage
        : public detail::ParameterChangedMessage<MessageType::HardwareSourceParameterChanged>,
          public controls::HardwareSourceParameter
    {
      bool m_isLocalEnabled = {};
      bool m_shouldSendMidi = true;
    };

    struct HardwareSourceSendParameterChangedMessage
        : public detail::ParameterChangedMessage<MessageType::HardwareSourceSendParameterChanged>,
          public controls::HardwareSourceSendParameter
    {
      controls::Id m_siblingId = controls::Id::None;
      bool m_isLocalEnabled = {};
    };

    struct HardwareAmountParameterChangedMessage
        : public detail::ParameterChangedMessage<MessageType::HardwareAmountParameterChanged>,
          public controls::HardwareAmountParameter
    {
    };

    struct MacroControlParameterChangedMessage
        : public detail::ParameterChangedMessage<MessageType::MacroControlParameterChanged>,
          public controls::MacroControlParameter
    {
    };

    struct MacroTimeParameterChangedMessage
        : public detail::ParameterChangedMessage<MessageType::MacroTimeParameterChanged>,
          public controls::MacroTimeParameter
    {
    };

    struct GlobalModulateableParameterChangedMessage
        : public detail::ParameterChangedMessage<MessageType::GlobalModulateableParameterChanged>,
          public controls::GlobalModulateableParameter,
          public detail::ModulateableChangedMessage
    {
      static constexpr VoiceGroup m_voiceGroup = VoiceGroup::Global;  // necessary?
    };

    struct GlobalUnmodulateableParameterChangedMessage
        : public detail::ParameterChangedMessage<MessageType::GlobalUnmodulateableParameterChanged>,
          public controls::GlobalUnmodulateableParameter
    {
      static constexpr VoiceGroup m_voiceGroup = VoiceGroup::Global;  // necessary?
    };

    struct PolyphonicModulateableParameterChangedMessage
        : public detail::ParameterChangedMessage<MessageType::PolyphonicModulateableParameterChanged>,
          public controls::PolyphonicModulateableParameter,
          public detail::ModulateableChangedMessage,
          public detail::LocalChangedMessage
    {
    };

    struct PolyphonicUnmodulateableParameterChangedMessage
        : public detail::ParameterChangedMessage<MessageType::PolyphonicUnmodulateableParameterChanged>,
          public controls::PolyphonicUnmodulateableParameter,
          public detail::LocalChangedMessage
    {
    };

    struct MonophonicModulateableParameterChangedMessage
        : public detail::ParameterChangedMessage<MessageType::MonophonicModulateableParameterChanged>,
          public controls::MonophonicModulateableParameter,
          public detail::ModulateableChangedMessage,
          public detail::LocalChangedMessage
    {
    };

    struct MonophonicUnmodulateableParameterChangedMessage
        : public detail::ParameterChangedMessage<MessageType::MonophonicUnmodulateableParameterChanged>,
          public controls::MonophonicUnmodulateableParameter,
          public detail::LocalChangedMessage
    {
    };

  }  // namespace nltools::msg

}  // namespace nltools
