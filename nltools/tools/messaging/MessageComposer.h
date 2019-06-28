#pragma once

#include "Messaging.h"

namespace nltools
{
  namespace msg
  {

    class MessageComposer
    {
     public:
      MessageComposer(Senders s, Receivers r, MessageType t);
      ~MessageComposer();

      void add(const uint16_t &v);
      void add(const float &v);
      void send();

     private:
      Senders m_sender;
      Receivers m_receiver;
      MessageType m_msgType;

      std::unique_ptr<Message> m_theMessage;
    };
  }
}
