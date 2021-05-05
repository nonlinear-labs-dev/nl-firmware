#include "UpdateContentManager.h"
#include <http/WebSocketRequest.h>

void UpdateContentManager::connectWebSocket(SoupWebsocketConnection *connection)
{
  using namespace std::string_literals;

  g_signal_connect(connection, "message", G_CALLBACK(&UpdateContentManager::onWebSocketMessage), this);
  m_webSockets.emplace_back(std::make_shared<WebsocketConnection>(connection));
}

void UpdateContentManager::onWebSocketMessage(SoupWebsocketConnection *self, gint type, GBytes *message,
                                              UpdateContentManager *pThis)
{
  std::shared_ptr<WebSocketRequest> request(new WebSocketRequest(self, message));
  pThis->handleRequest(request);
}

void UpdateContentManager::handleRequest(const std::shared_ptr<WebSocketRequest> &request)
{

  //request->
}

UpdateContentManager::WebsocketConnection::WebsocketConnection(SoupWebsocketConnection *c)
{
  ws = c;
  g_object_ref(ws);
}

UpdateContentManager::WebsocketConnection::~WebsocketConnection()
{
  g_object_unref(ws);
}

SoupWebsocketConnection *UpdateContentManager::WebsocketConnection::getConnection()
{
  return ws;
}
