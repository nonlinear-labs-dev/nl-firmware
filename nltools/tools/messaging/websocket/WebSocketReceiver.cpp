#include "WebSocketReceiver.h"

namespace nltools
{
  namespace msg
  {
    namespace ws
    {

      WebSocketReceiver::WebSocketReceiver(Callback cb, int port)
          : Receiver(cb)
      {
      }
    }
  }
}
