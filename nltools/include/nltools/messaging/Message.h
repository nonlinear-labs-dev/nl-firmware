#pragma once

#include "Messaging.h"
#include <nltools/Types.h>
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
      MacroControlChangedMessage(tID id = 0, tControlPosition pos = 0.0, tControlPosition time = 0.0,
                                 bool locked = false, VoiceGroup vg = VoiceGroup::I)
          : lock{ locked }
          , parameterId{ id }
          , controlPosition{ pos }
          , smoothingTime{ time }
          , voiceGroup{ vg }
      {
      }

      bool lock;
      tID parameterId;
      tControlPosition controlPosition;
      tControlPosition smoothingTime;

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
      template <> inline LPCMessage deserialize<LPCMessage>(const SerializedMessage &s)
      {
        LPCMessage ret;
        gsize numBytes = 0;
        auto data = reinterpret_cast<const uint8_t *>(s->get_data(numBytes));
        ret.message = Glib::Bytes::create(data + 2, numBytes - 2);
        return ret;
      }

      template <> inline SerializedMessage serialize<LPCMessage>(const LPCMessage &msg)
      {
        gsize numBytes = 0;
        auto data = reinterpret_cast<const uint8_t *>(msg.message->get_data(numBytes));
        auto scratch = reinterpret_cast<uint16_t *>(g_malloc(numBytes + 2));
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
        int32_t id;
        float controlPosition = 0;
      };

      struct RibbonParameter : Parameter
      {
        RibbonTouchBehaviour ribbonTouchBehaviour;
        RibbonReturnMode ribbonReturnMode;
      };

      struct PedalParameter : Parameter
      {
        PedalModes pedalMode;
        ReturnMode returnMode;
      };

      struct AftertouchParameter : Parameter
      {
        ReturnMode returnMode;
      };

      struct BenderParameter : Parameter
      {
        ReturnMode returnMode;
      };

      struct MacroParameter : Parameter
      {
      };

      struct ModulateableParameter : Parameter
      {
        MacroControls mc = MacroControls::NONE;
        float modulationAmount = 0;
      };

      struct UnmodulatebaleParameter : Parameter
      {
      };

      struct MonoParameter : Parameter
      {
      };
    }

    struct SinglePresetMessage : Message<MessageType::SinglePreset>
    {
      std::array<ParameterGroups::PedalParameter, 4> pedals;
      std::array<ParameterGroups::AftertouchParameter, 1> aftertouch;
      std::array<ParameterGroups::RibbonParameter, 2> ribbons;
      std::array<ParameterGroups::BenderParameter, 1> bender;
      std::array<ParameterGroups::MacroParameter, 4> macros;
      std::array<ParameterGroups::ModulateableParameter, 89> modulateables;
      std::array<ParameterGroups::UnmodulatebaleParameter, 138> unmodulateables;
      std::array<ParameterGroups::MonoParameter, 4> monos;
    };

    struct SplitPresetMessage : Message<MessageType::SplitPreset>
    {
      std::array<std::array<ParameterGroups::PedalParameter, 4>, 2> pedals;
      std::array<std::array<ParameterGroups::AftertouchParameter, 1>, 2> aftertouch;
      std::array<std::array<ParameterGroups::RibbonParameter, 2>, 2> ribbons;
      std::array<std::array<ParameterGroups::BenderParameter, 1>, 2> bender;
      std::array<std::array<ParameterGroups::MacroParameter, 4>, 2> macros;
      std::array<std::array<ParameterGroups::ModulateableParameter, 89>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulatebaleParameter, 138>, 2> unmodulateables;
      std::array<std::array<ParameterGroups::MonoParameter, 4>, 2> monos;
    };

    struct LayerPresetMessage : Message<MessageType::LayerPreset>
    {
      std::array<std::array<ParameterGroups::PedalParameter, 4>, 2> pedals;
      std::array<std::array<ParameterGroups::AftertouchParameter, 1>, 2> aftertouch;
      std::array<std::array<ParameterGroups::RibbonParameter, 2>, 2> ribbons;
      std::array<std::array<ParameterGroups::BenderParameter, 1>, 2> bender;
      std::array<std::array<ParameterGroups::MacroParameter, 4>, 2> macros;
      std::array<std::array<ParameterGroups::ModulateableParameter, 89>, 2> modulateables;
      std::array<std::array<ParameterGroups::UnmodulatebaleParameter, 138>, 2> unmodulateables;
      std::array<ParameterGroups::MonoParameter, 4> monos;
    };
  }
}