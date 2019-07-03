#pragma once

#include "Messaging.h"

namespace nltools
{
  namespace msg
  {
    struct ParameterChangedMessage
    {
      static const MessageType theType = MessageType::Parameter;

      ParameterChangedMessage(uint16_t id = 0, double controlPosition = 0)
          : parameterId(id)
          , controlPosition(controlPosition)
      {
      }

      MessageType type = theType;
      uint16_t parameterId;
      double controlPosition;
    };

    struct PresetChangedMessage
    {
      static const MessageType theType = MessageType::Preset;

      MessageType type = theType;
      double values[380];
    };
  }
}
