#include "AudioEngineProxy.h"
#include <messaging/Message.h>

void AudioEngineProxy::sendParameter(uint16_t id, tControlPositionValue value)
{
  using namespace nltools::msg;
  ParameterChangedMessage message;
  message.parameterId = id;
  message.controlPosition = value;
  send(Receivers::AudioEngine, message);
}
