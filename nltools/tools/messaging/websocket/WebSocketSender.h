#pragma once

#include <tools/messaging/Sender.h>

namespace nltools
{
  namespace msg
  {
    namespace ws
    {
      class WebSocketSender : public Sender
      {
       public:
        WebSocketSender(const std::string &targetMachine, int port);
      };
    }
  }
}
