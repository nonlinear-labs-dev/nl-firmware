#pragma once

#include <nltools/messaging/Message.h>
#include <parameters/messaging/ParameterMessageFactory.h>
#include <groups/MonoGroup.h>
#include "playground.h"

namespace UNDO
{
  class Transaction;
}

class EditBuffer;

class AudioEngineProxy
{
 public:
  AudioEngineProxy();

  template <typename tParameter> auto createAndSendParameterMessage(const tParameter* parameter)
  {
    if(!m_suppressParamChanges)
      sendParameterMessage(createMessage<tParameter>(parameter));
  }

  template <typename tParameter> auto createMessage(const tParameter* parameter)
  {
    return ParameterMessageFactory::createMessage<tParameter>(parameter);
  }

  template <class tMessage> void sendParameterMessage(const tMessage& msg)
  {
    if(!m_suppressParamChanges)
      send(sendToEndPoint, msg);
  }

  template <typename tSettingMessage> void sendSettingMessage(const tSettingMessage& msg)
  {
    send(sendToEndPoint, msg);
  }

  void sendEditBuffer();

  void setReceiver(nltools::msg::EndPoint endPoint);

  void freezeParameterMessages();
  void thawParameterMessages(bool send);

  static nltools::msg::LayerPresetMessage createLayerEditBufferMessage(const EditBuffer& eb);
  static nltools::msg::SplitPresetMessage createSplitEditBufferMessage(const EditBuffer& eb);
  static nltools::msg::SinglePresetMessage createSingleEditBufferMessage(const EditBuffer& eb);

 private:
  nltools::msg::EndPoint sendToEndPoint = nltools::msg::EndPoint::AudioEngine;
  uint m_suppressParamChanges = 0;

  static void fillMonoPart(nltools::msg::ParameterGroups::MonoGroup& monoGroup, ParameterGroup* const& g);
  static void fillUnisonPart(nltools::msg::ParameterGroups::UnisonGroup& unisonGroup, ParameterGroup* const& g);
};
