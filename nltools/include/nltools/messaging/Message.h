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
  }
}
