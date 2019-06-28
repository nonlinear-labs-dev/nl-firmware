#pragma once

#include <tools/messaging/Receiver.h>

namespace nltools
{
  namespace msg
  {
    namespace ws
    {

      class WebSocketReceiver : public Receiver
      {
       public:
        WebSocketReceiver(Callback cb, int port);
      };
    }
  }
}
