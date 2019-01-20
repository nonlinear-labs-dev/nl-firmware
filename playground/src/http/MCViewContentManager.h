#pragma once
#include "ContentManager.h"

class MCViewContentManager
{
 public:
  MCViewContentManager();
  void connectWebSocket(SoupWebsocketConnection *connection);
  static void onWebSocketMessage(SoupWebsocketConnection *self, gint type, GBytes *message,
                                 MCViewContentManager *pThis);

  void sendToAllWebsockets(const Glib::ustring& message) const;

  void handleRequest(std::shared_ptr<WebSocketRequest> request);

  struct WebsocketConnection
  {
   public:
    explicit WebsocketConnection(SoupWebsocketConnection *c);
    ~WebsocketConnection();
    SoupWebsocketConnection *getConnection();
   private:
    SoupWebsocketConnection *ws;
  };

  std::vector<std::shared_ptr<WebsocketConnection>> m_webSockets;
};
