#pragma once

#include <nltools/messaging/Message.h>
#include "playground.h"

namespace UNDO
{
  class Transaction;
}

class AudioEngineProxy
{
 public:
  AudioEngineProxy();

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
