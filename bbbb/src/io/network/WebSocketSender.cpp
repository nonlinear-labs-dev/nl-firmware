#include <io/network/WebSocketSender.h>
#include <io/network/WebSocketServer.h>
#include <Application.h>
#include <string.h>

WebSocketSender::WebSocketSender(Domain d) :
    m_domain(d)
{
}

WebSocketSender::~WebSocketSender()
{
}

void WebSocketSender::send(tMessage msg)
{
  gsize len = 0;
  auto data = reinterpret_cast<const uint8_t*>(msg->get_data(len));
  auto dataWithHeader = new uint8_t[len + 1];
  dataWithHeader[0] = (uint8_t)m_domain;
  memcpy(dataWithHeader + 1, data, len);
  Application::get().getWebsocketServer()->sendMessage(Glib::Bytes::create(dataWithHeader, len + 1));
}
