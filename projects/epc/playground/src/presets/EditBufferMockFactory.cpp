#ifdef _DEVELOPMENT_PC
#include "EditBufferMockFactory.h"

nltools2::msg::SinglePresetMessage EditBufferMockFactory::initialMessage()
{
  nltools2::msg::SinglePresetMessage ret {};
  return ret;
}

nltools2::msg::SplitPresetMessage EditBufferMockFactory::splitMessage()
{
  nltools2::msg::SplitPresetMessage ret {};
  return ret;
}

nltools2::msg::SinglePresetMessage EditBufferMockFactory::splitToSingleMessage()
{
  nltools2::msg::SinglePresetMessage ret {};
  return ret;
}

nltools2::msg::LayerPresetMessage EditBufferMockFactory::layerMessage()
{
  nltools2::msg::LayerPresetMessage ret {};
  return ret;
}

nltools2::msg::SinglePresetMessage EditBufferMockFactory::layerToSingleMessage()
{
  nltools2::msg::SinglePresetMessage ret {};
  return ret;
}

#endif