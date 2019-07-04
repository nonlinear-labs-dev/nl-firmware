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
      ParameterChangedMessage(uint16_t id = 0, double controlPosition = 0)
          : parameterId(id)
          , controlPosition(controlPosition)
      {
      }

      uint16_t parameterId;
      double controlPosition;
    };

    struct PresetChangedMessage : Message<MessageType::Preset>
    {
      double values[380];
    };
  }
}
