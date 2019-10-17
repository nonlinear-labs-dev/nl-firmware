#pragma once

#include <nltools/messaging/Message.h>
#include <parameters/messaging/ParameterMessageFactory.h>
#include "playground.h"

namespace UNDO
{
  class Transaction;
}

class AudioEngineProxy
{
 public:
  AudioEngineProxy();

  template<typename tParameter>
  auto createAndSendParameterMessage(const tParameter* parameter) {
    sendParameterMessage(createMessage<tParameter>(parameter));
  }

  template<typename tParameter>
  auto createMessage(const tParameter* parameter) {
    return ParameterMessageFactory::createMessage<tParameter>(parameter);
  }

  template<class tMessage>
  void sendParameterMessage(const tMessage &msg)
  {
    send(nltools::msg::EndPoint::AudioEngine, msg);
  }

  void sendEditBuffer();
  void toggleSuppressParameterChanges(UNDO::Transaction *transaction);

  void sendLayerEditBuffer();
  void sendSplitEditBuffer();
  void sendSingleEditBuffer();

 private:
  bool m_suppressParamChanges = false;
};
