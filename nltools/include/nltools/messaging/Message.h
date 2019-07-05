#pragma once

#include "Messaging.h"

namespace nltools
{
  namespace msg
  {
    template <MessageType msgType> struct Message
    {
      static const MessageType theType = msgType;
      MessageType type = msgType;
    };

    struct ParameterChangedMessage : Message<MessageType::Parameter>
    {
      ParameterChangedMessage(uint16_t id = 0, float controlPosition = 0)
          : parameterId(id)
          , controlPosition(controlPosition)
      {
      }

      uint16_t parameterId;
      float controlPosition;
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

    struct SetPresetMessage : Message<MessageType::Preset>
    {
      enum class MCs
      {
        None,
        A,
        B,
        C,
        D

      };

      struct Parameter
      {
        int32_t id;
        float controlPosition = 0;
      };

      struct RibbonParameter : Parameter
      {
      };

      struct PedalParameter : Parameter
      {
      };

      struct AftertouchParameter : Parameter
      {
      };

      struct BenderParameter : Parameter
      {
      };

      struct MacroParameter : Parameter
      {
      };

      struct ModulateableParameter : Parameter
      {
        MCs mc = MCs::None;
        float modulationAmount = 0;
      };

      struct UnmodulatebaleParameter : Parameter
      {
      };

      std::array<PedalParameter, 4> pedals;
      std::array<AftertouchParameter, 1> aftertouch;
      std::array<RibbonParameter, 2> ribbons;
      std::array<BenderParameter, 1> bender;
      std::array<MacroParameter, 4> macros;
      std::array<ModulateableParameter, 1000> modulateables;
      std::array<UnmodulatebaleParameter, 1000> unmodulateables;
    };
  }
}
