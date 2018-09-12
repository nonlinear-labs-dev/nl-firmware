#pragma once

#include <io/Receiver.h>

class WebSocketReceiver : public Receiver
{
 public:
  WebSocketReceiver(Domain domain);
  virtual ~WebSocketReceiver();
};
