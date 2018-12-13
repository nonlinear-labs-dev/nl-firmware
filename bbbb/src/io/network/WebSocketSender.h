#pragma once

#include <io/Sender.h>

class WebSocketSender : public Sender
{
 public:
  WebSocketSender(Domain domain);
  virtual ~WebSocketSender();

  void send(tMessage msg) override;

 private:
  Domain m_domain;
};
