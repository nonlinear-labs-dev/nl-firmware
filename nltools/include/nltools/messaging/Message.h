#pragma once

#include "Messaging.h"
#include <nltools/Types.h>
#include <nltools/Testing.h>
#include <cstring>

namespace nltools
{
  namespace msg
  {

    template <MessageType msgType> struct Message
    {
      static const MessageType theType = msgType;
      MessageType type = msgType;
    };

    using tID = int;
    using tControlPosition = double;

    namespace Setting
    {
      struct NoteShiftMessage : Message<MessageType::NoteShiftSetting>
      {
        NoteShiftMessage()
            : m_shift{ 0 }
        {
        }

        NoteShiftMessage(int offset)
            : m_shift{ offset }
        {
        }

        int m_shift{};
      };

      struct PresetGlitchMessage : Message<MessageType::PresetGlitchSetting>
      {
        PresetGlitchMessage()
            : m_enabled{ false }
        {
        }

        PresetGlitchMessage(bool enabled)
            : m_enabled{ enabled }
        {
        }

        bool m_enabled{};
      };

      struct TransitionTimeMessage : Message<MessageType::TransitionTimeSetting>
      {
        TransitionTimeMessage()
            : m_value{ 0.0 }
        {
        }

        TransitionTimeMessage(float value)
            : m_value{ value }
        {
        }

        float m_value{};
      };

      struct EditSmoothingTimeMessage : Message<MessageType::EditSmoothingTimeSetting>
      {
        EditSmoothingTimeMessage()
            : m_time{ 0 }
        {
        }

        EditSmoothingTimeMessage(float time)
            : m_time{ time }
        {
        }

        float m_time{};
      };
    }

    struct HWSourceChangedMessage : Message<MessageType::HWSourceParameter>
    {
      HWSourceChangedMessage(tID id = 0, tControlPosition controlPosition = 0, ReturnMode mode = ReturnMode::None,
                             bool locked = false, VoiceGroup vg = VoiceGroup::I)
          : parameterId{ id }
          , controlPosition{ controlPosition }
          , returnMode{ mode }
          , lock(locked)
          , voiceGroup{ vg }
      {
      }

      tID parameterId;
      tControlPosition controlPosition;
      ReturnMode returnMode;
      bool lock;

      VoiceGroup voiceGroup;
    };

    struct HWAmountChangedMessage : Message<MessageType::HWAmountParameter>
    {
      HWAmountChangedMessage(tID id = 0, tControlPosition pos = 0.0, bool locked = false, VoiceGroup vg = VoiceGroup::I)
          : parameterId{ id }
          , controlPosition{ pos }
          , lock{ locked }
          , voiceGroup{ vg }
      {
      }

      tID parameterId;
      tControlPosition controlPosition;
      bool lock;

      VoiceGroup voiceGroup;
    };

    struct MacroControlChangedMessage : Message<MessageType::MacroControlParameter>
    {
      MacroControlChangedMessage(tID id = 0, tControlPosition pos = 0.0, bool locked = false,
                                 VoiceGroup vg = VoiceGroup::I)
          : lock{ locked }
          , parameterId{ id }
          , controlPosition{ pos }
          , voiceGroup{ vg }
      {
      }

      bool lock;
      tID parameterId;
      tControlPosition controlPosition;

      VoiceGroup voiceGroup;
    };

    struct UnmodulateableParameterChangedMessage : Message<MessageType::UnmodulateableParameter>
    {
      UnmodulateableParameterChangedMessage(tID id = 0, tControlPosition controlPosition = 0, bool locked = false,
                                            VoiceGroup vg = VoiceGroup::I)
          : lock{ locked }
          , parameterId(id)
          , controlPosition(controlPosition)
          , voiceGroup{ vg }
      {
      }

      bool lock;
      tID parameterId;
      tControlPosition controlPosition;

      VoiceGroup voiceGroup;
    };

    struct ModulateableParameterChangedMessage : Message<MessageType::ModulateableParameter>
    {
      ModulateableParameterChangedMessage(tID id = 0, tControlPosition pos = 0.0,
                                          MacroControls source = MacroControls::NONE, tControlPosition amount = 0.0,
                                          tControlPosition upper = 0.0, tControlPosition lower = 0.0,
                                          bool locked = false, VoiceGroup vg = VoiceGroup::I)
          : lock{ locked }
          , parameterId{ id }
          , controlPosition{ pos }
          , sourceMacro{ source }
          , mcAmount{ amount }
          , mcUpper{ upper }
          , mcLower{ lower }
          , voiceGroup{ vg }
      {
      }

      bool lock;
      tID parameterId;
      tControlPosition controlPosition;
      MacroControls sourceMacro;
      tControlPosition mcAmount;
      tControlPosition mcUpper;
      tControlPosition mcLower;

      VoiceGroup voiceGroup;
    };

    struct RotaryChangedMessage : Message<MessageType::RotaryChanged>
    {
      int8_t increment;
    };

    struct ButtonChangedMessage : Message<MessageType::ButtonChanged>
    {
      int8_t buttonId;
      bool pressed;
    };

    struct SetRibbonLEDMessage : Message<MessageType::SetRibbonLED>
    {
      uint8_t id;
      uint8_t brightness;
    };

    struct SetPanelLEDMessage : Message<MessageType::SetPanelLED>
    {
      uint8_t id;
      bool on;
    };

    struct SetOLEDMessage : Message<MessageType::SetOLED>
    {
      uint8_t pixels[256][96];
    };

    struct LPCMessage : Message<MessageType::LPC>
    {
      Glib::RefPtr<Glib::Bytes> message;
    };

    struct PingMessage : Message<MessageType::Ping>
    {
    };

    namespace detail
    {
      // default (de)serialization for messages, may be specialized for more compilcated types:
      template <> inline LPCMessage deserialize<LPCMessage>(const SerializedMessage& s)
      {
        LPCMessage ret;
        gsize numBytes = 0;
        auto data = reinterpret_cast<const uint8_t*>(s->get_data(numBytes));
        ret.message = Glib::Bytes::create(data + 2, numBytes - 2);
        return ret;
      }

      template <> inline SerializedMessage serialize<LPCMessage>(const LPCMessage& msg)
      {
        gsize numBytes = 0;
        auto data = reinterpret_cast<const uint8_t*>(msg.message->get_data(numBytes));
        auto scratch = reinterpret_cast<uint16_t*>(g_malloc(numBytes + 2));
        scratch[0] = static_cast<uint16_t>(MessageType::LPC);
        std::memcpy(&scratch[1], data, numBytes);
        auto bytes = g_bytes_new_take(scratch, numBytes + 2);
        return Glib::wrap(bytes);
      }
    }

    namespace ParameterGroups
    {
      struct Parameter
      {
        uint16_t id{};
        double controlPosition = 0;
        bool locked = false;
      };

      struct RibbonParameter : Parameter
      {
        RibbonTouchBehaviour ribbonTouchBehaviour{};
        RibbonReturnMode ribbonReturnMode{};
      };

      struct PedalParameter : Parameter
      {
        PedalModes pedalMode{};
        ReturnMode returnMode{};
      };

      struct AftertouchParameter : Parameter
      {
        ReturnMode returnMode{};
      };

      struct BenderParameter : Parameter
      {
        ReturnMode returnMode{};
      };

      struct MacroParameter : Parameter
      {
      };

      struct ModulateableParameter : Parameter
      {
        MacroControls mc = MacroControls::NONE;
        double modulationAmount = 0;
      };

      struct UnmodulatebaleParameter : Parameter
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

      struct SplitPoint : Parameter
      {
      };

      inline bool operator==(const Parameter& lhs, const Parameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition;
      }

      inline bool operator==(const ModulateableParameter& lhs, const ModulateableParameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition && lhs.mc == rhs.mc
            && lhs.modulationAmount == rhs.modulationAmount;
      }

      inline bool operator==(const AftertouchParameter& lhs, const AftertouchParameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition && lhs.returnMode == rhs.returnMode;
      }

      inline bool operator==(const BenderParameter& lhs, const BenderParameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition && lhs.returnMode == rhs.returnMode;
      }

      inline bool operator==(const PedalParameter& lhs, const PedalParameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition && lhs.pedalMode == rhs.pedalMode
            && lhs.returnMode == rhs.returnMode;
      }

      inline bool operator==(const RibbonParameter& lhs, const RibbonParameter& rhs)
      {
        return lhs.id == rhs.id && lhs.controlPosition == rhs.controlPosition
            && lhs.ribbonTouchBehaviour == rhs.ribbonTouchBehaviour && lhs.ribbonReturnMode == rhs.ribbonReturnMode;
      }
    }

    struct SinglePresetMessage : Message<MessageType::SinglePreset>
    {
      std::array<ParameterGroups::MacroParameter, 6> macros;
      std::array<ParameterGroups::ModulateableParameter, 97> modulateables;
      std::array<ParameterGroups::UnmodulatebaleParameter, 107> unmodulateables;
      std::array<ParameterGroups::HardwareSourceParameter, 8> hwsources;
      std::array<ParameterGroups::HardwareAmountParameter, 48> hwamounts;
      ParameterGroups::UnmodulatebaleParameter unisonVoices;
      std::array<ParameterGroups::GlobalParameter, 14> globalparams;
    };

    struct SplitPresetMessage : Message<MessageType::SplitPreset>
    {
      std::array<std::array<ParameterGroups::MacroParameter, 6>, 2> macros;
      std::array<std::array<ParameterGroups::ModulateableParameter, 97>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulatebaleParameter, 107>, 2> unmodulateables;
      std::array<std::array<ParameterGroups::HardwareAmountParameter, 48>, 2> hwamounts;

      std::array<ParameterGroups::UnmodulatebaleParameter, 2> unisonVoices;

      std::array<ParameterGroups::HardwareSourceParameter, 8> hwsources;
      std::array<ParameterGroups::GlobalParameter, 14> globalparams;
      ParameterGroups::SplitPoint splitpoint;
    };

    struct LayerPresetMessage : Message<MessageType::LayerPreset>
    {
      std::array<std::array<ParameterGroups::MacroParameter, 6>, 2> macros;
      std::array<std::array<ParameterGroups::ModulateableParameter, 97>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulatebaleParameter, 107>, 2> unmodulateables;
      std::array<std::array<ParameterGroups::HardwareAmountParameter, 48>, 2> hwamounts;

      std::array<ParameterGroups::UnmodulatebaleParameter, 2> unisonVoices;

      std::array<ParameterGroups::HardwareSourceParameter, 8> hwsources;
      std::array<ParameterGroups::GlobalParameter, 14> globalparams;
    };
  }
}