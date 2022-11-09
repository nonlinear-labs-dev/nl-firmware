#pragma once

#include "Parameters.h"

namespace nltools {

    namespace msg {

        // generic structures

        template<MessageType T>
        struct ParameterChangedMessage {
            static_assert(
                T == MessageType::HardwareSourceParameterChanged || T == MessageType::HardwareSourceSendParameterChanged ||
                T == MessageType::HardwareAmountParameterChanged ||
                T == MessageType::MacroControlParameterChanged || T == MessageType::MacroTimeParameterChanged ||
                T == MessageType::GlobalModulateableParameterChanged || T == MessageType::GlobalUnmodulateableParameterChanged ||
                T == MessageType::LocalModulateableParameterChanged || T == MessageType::LocalUnmodulateableParameterChanged ||
                T == MessageType::PolyphonicModulateableParameterChanged || T == MessageType::PolyphonicUnmodulateableParameterChanged ||
                T == MessageType::MonophonicModulateableParameterChanged || T == MessageType::MonophonicUnmodulateableParameterChanged,
                "MessageType must fit category ParameterChanged"
            );
            constexpr static MessageType getType()
            {
              return T;
            }
        };

        struct ModulateableChangedMessage {
            ParameterData::Position m_modulationUpper = {};
            ParameterData::Position m_modulationLower = {};
        };

        struct LocalChangedMessage {
            VoiceGroup m_voiceGroup = VoiceGroup::Invalid;
        };

        // usable messages

        struct HardwareSourceParameterChangedMessage
            : public ParameterChangedMessage<MessageType::HardwareSourceParameterChanged>
            , public ParameterData::HardwareSourceParameter
        {
            bool m_isLocalEnabled = {};
        };

        struct HardwareSourceSendParameterChangedMessage
            : public ParameterChangedMessage<MessageType::HardwareSourceSendParameterChanged>
            , public ParameterData::HardwareSourceSendParameter
        {
            ParameterData::Id m_siblingId = ParameterData::Id::None;
            bool m_isLocalEnabled = {};
        };

        struct MacroControlParameterChangedMessage
            : public ParameterChangedMessage<MessageType::MacroControlParameterChanged>
            , public ParameterData::MacroControlParameter
        {};

        struct MacroTimeParameterChangedMessage
            : public ParameterChangedMessage<MessageType::MacroTimeParameterChanged>
            , public ParameterData::MacroTimeParameter
        {};

        struct GlobalModulateableParameterChangedMessage
            : public ParameterChangedMessage<MessageType::GlobalModulateableParameterChanged>
            , public ParameterData::GlobalModulateableParameter
            , public ModulateableChangedMessage
        {
            static constexpr VoiceGroup m_voiceGroup = VoiceGroup::Global; // necessary?
        };

        struct GlobalUnmodulateableParameterChangedMessage
            : public ParameterChangedMessage<MessageType::GlobalUnmodulateableParameterChanged>
            , public ParameterData::GlobalUnmodulateableParameter
        {
            static constexpr VoiceGroup m_voiceGroup = VoiceGroup::Global; // necessary?
        };

        // todo: deprecate
        struct LocalModulateableParameterChangedMessage
            : public ParameterChangedMessage<MessageType::LocalModulateableParameterChanged>
            , public ParameterData::LocalModulateableParameter
            , public ModulateableChangedMessage
            , public LocalChangedMessage
        {};

        // todo: deprecate
        struct LocalUnmodulateableParameterChangedMessage
            : public ParameterChangedMessage<MessageType::LocalUnmodulateableParameterChanged>
            , public ParameterData::LocalUnmodulateableParameter
            , public LocalChangedMessage
        {};

        struct PolyphonicModulateableParameterChangedMessage
            : public ParameterChangedMessage<MessageType::PolyphonicModulateableParameterChanged>
            , public ParameterData::PolyphonicModulateableParameter
            , public ModulateableChangedMessage
            , public LocalChangedMessage
        {};

        struct PolyphonicUnmodulateableParameterChangedMessage
            : public ParameterChangedMessage<MessageType::PolyphonicUnmodulateableParameterChanged>
            , public ParameterData::PolyphonicUnmodulateableParameter
            , public LocalChangedMessage
        {};

        struct MonophonicModulateableParameterChangedMessage
            : public ParameterChangedMessage<MessageType::MonophonicModulateableParameterChanged>
            , public ParameterData::MonophonicModulateableParameter
            , public ModulateableChangedMessage
            , public LocalChangedMessage
        {};

        struct MonophonicUnmodulateableParameterChangedMessage
            : public ParameterChangedMessage<MessageType::MonophonicUnmodulateableParameterChanged>
            , public ParameterData::MonophonicUnmodulateableParameter
            , public LocalChangedMessage
        {};

    } // namespace nltools::msg

} // namespace nltools
