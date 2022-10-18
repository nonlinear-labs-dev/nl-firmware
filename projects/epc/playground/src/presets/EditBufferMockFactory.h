#pragma once
#ifdef _DEVELOPMENT_PC
#include <nltools/messaging/Message.h>
#include "PresetMessages.h"

class EditBufferMockFactory
{
 public:
  static nltools2::msg::SinglePresetMessage initialMessage();
  static nltools2::msg::SplitPresetMessage splitMessage();
  static nltools2::msg::SinglePresetMessage splitToSingleMessage();
  static nltools2::msg::LayerPresetMessage layerMessage();
  static nltools2::msg::SinglePresetMessage layerToSingleMessage();
};
#endif
