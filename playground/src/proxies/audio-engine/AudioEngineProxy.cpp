#include "AudioEngineProxy.h"
#include <nltools/messaging/Message.h>

void AudioEngineProxy::sendParameter(uint16_t id, tControlPositionValue value)
{
  using namespace nltools::msg;
  send(Participants::AudioEngine, ParameterChangedMessage(id, value));
}
