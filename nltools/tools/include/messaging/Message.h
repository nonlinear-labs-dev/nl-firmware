#pragma once

#include "Messaging.h"

namespace nltools
{
  namespace msg
  {

    struct ParameterChangedMessage
    {
      MessageType type = MessageType::Parameter;
      uint16_t parameterId;
      double controlPosition;
    };

    struct PresetChangedMessage
    {
      MessageType type = MessageType::Parameter;
      double values[380];
    };
  }
}
