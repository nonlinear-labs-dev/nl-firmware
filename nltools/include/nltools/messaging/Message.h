#pragma once

#include "Messaging.h"
#include <nltools/Types.h>
#include <nltools/Testing.h>
#include <cstring>

namespace nltools
{
  namespace msg
  {

    using tID = int;
    using tControlPosition = double;

    namespace Setting
    {
      struct NoteShiftMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::NoteShiftSetting;
        }

        int m_shift;
      };

      struct PresetGlitchMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::PresetGlitchSetting;
        }
        bool m_enabled;
      };

      struct TransitionTimeMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::TransitionTimeSetting;
        }
        float m_value;
      };

      struct EditSmoothingTimeMessage
      {
        constexpr static MessageType getType()
        {
          return MessageType::EditSmoothingTimeSetting;
        }
        float m_time;
      };
    }

    struct HWSourceChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::HWSourceParameter;
      }

      tID parameterId;
      tControlPosition controlPosition;
      ReturnMode returnMode;
      bool lock;
      VoiceGroup voiceGroup;
    };

    struct HWAmountChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::HWAmountParameter;
      }

      tID parameterId;
      tControlPosition controlPosition;
      bool lock;
      VoiceGroup voiceGroup;
    };

    struct MacroControlChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::MacroControlParameter;
      }

      tID parameterId;
      tControlPosition controlPosition;
      bool lock;
      VoiceGroup voiceGroup;
    };

    struct UnmodulateableParameterChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::UnmodulateableParameter;
      }

      tID parameterId;
      tControlPosition controlPosition;
      bool lock;
      VoiceGroup voiceGroup;
    };

    struct ModulateableParameterChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::ModulateableParameter;
      }

      tID parameterId;
      tControlPosition controlPosition;
      MacroControls sourceMacro;
      tControlPosition mcAmount;
      tControlPosition mcUpper;
      tControlPosition mcLower;
      bool lock;
      VoiceGroup voiceGroup;
    };

    struct RotaryChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::RotaryChanged;
      }
      int8_t increment;
    };

    struct ButtonChangedMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::ButtonChanged;
      }
      int8_t buttonId;
      bool pressed;
    };

    struct SetRibbonLEDMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::SetRibbonLED;
      }
      uint8_t id;
      uint8_t brightness;
    };

    struct SetPanelLEDMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::SetPanelLED;
      }
      uint8_t id;
      bool on;
    };

    struct SetOLEDMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::SetOLED;
      }
      uint8_t pixels[256][96];
    };

    struct LPCMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::LPC;
      }
      Glib::RefPtr<Glib::Bytes> message;
    };

    struct PingMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::Ping;
      }
    };

    namespace detail
    {
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

      struct SplitPoint : Parameter
      {
      };
    }

    struct SinglePresetMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::SinglePreset;
      }

      std::array<ParameterGroups::MacroParameter, 6> macros;
      std::array<ParameterGroups::UnmodulateableParameter, 6> macrotimes;

      std::array<ParameterGroups::ModulateableParameter, 100> modulateables;
      std::array<ParameterGroups::UnmodulateableParameter, 91> unmodulateables;

      std::array<ParameterGroups::HardwareSourceParameter, 8> hwsources;
      std::array<ParameterGroups::HardwareAmountParameter, 48> hwamounts;
      ParameterGroups::UnmodulateableParameter unisonVoices;
      std::array<ParameterGroups::GlobalParameter, 14> globalparams;
    };

    struct SplitPresetMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::SplitPreset;
      }

      std::array<std::array<ParameterGroups::ModulateableParameter, 100>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulateableParameter, 91>, 2> unmodulateables;

      std::array<ParameterGroups::UnmodulateableParameter, 2> unisonVoices;

      std::array<ParameterGroups::HardwareSourceParameter, 8> hwsources;
      std::array<ParameterGroups::HardwareAmountParameter, 48> hwamounts;

      std::array<ParameterGroups::MacroParameter, 6> macros;
      std::array<ParameterGroups::UnmodulateableParameter, 6> macrotimes;

      std::array<ParameterGroups::GlobalParameter, 14> globalparams;
      ParameterGroups::SplitPoint splitpoint;
    };

    struct LayerPresetMessage
    {
      constexpr static MessageType getType()
      {
        return MessageType::LayerPreset;
      }

      std::array<ParameterGroups::HardwareSourceParameter, 8> hwsources;
      std::array<ParameterGroups::HardwareAmountParameter, 48> hwamounts;

      std::array<ParameterGroups::MacroParameter, 6> macros;
      std::array<ParameterGroups::UnmodulateableParameter, 6> macrotimes;

      std::array<std::array<ParameterGroups::ModulateableParameter, 100>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulateableParameter, 91>, 2> unmodulateables;

      std::array<ParameterGroups::UnmodulateableParameter, 2> unisonVoices;

      std::array<ParameterGroups::GlobalParameter, 14> globalparams;
    };
  }
}
