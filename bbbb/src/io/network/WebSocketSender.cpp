#include <io/network/WebSocketSender.h>
#include <io/network/WebSocketServer.h>
#include <Application.h>
#include <string.h>

WebSocketSender::WebSocketSender(Domain d)
    : m_domain(d)
{
}

WebSocketSender::~WebSocketSender()
{
}

void WebSocketSender::send(tMessage msg)
{
  gsize len = 0;
  auto data = reinterpret_cast<const uint8_t*>(msg->get_data(len));
  uint8_t dataWithHeader[len + 1];
  dataWithHeader[0] = static_cast<uint8_t>(m_domain);
  memcpy(dataWithHeader + 1, data, len);
  Application::get().getWebsocketServer()->sendMessage(Glib::Bytes::create(dataWithHeader, len + 1));
}
