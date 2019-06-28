#pragma once

#include <memory>

namespace nltools
{
  namespace msg
  {
    class Message;

    class Sender
    {
     public:
      Sender();
      virtual ~Sender();

      virtual void send(std::unique_ptr<Message> msg) = 0;
    };
  }
}
