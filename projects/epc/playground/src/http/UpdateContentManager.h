#pragma once
#include "ContentManager.h"

class UpdateContentManager : public ContentManager
{
 public:
  void connectWebSocket(SoupWebsocketConnection *connection);

  static void onWebSocketMessage(SoupWebsocketConnection *self, gint type, GBytes *message,
                                 UpdateContentManager *pThis);

  void handleRequest(const std::shared_ptr<WebSocketRequest> &request);

 private:
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
