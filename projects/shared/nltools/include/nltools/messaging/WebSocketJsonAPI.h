#pragma once

#include <nltools/threading/ContextBoundMessageQueue.h>
#include <nltools/nlohmann/json.hpp>
#include <functional>
#include <libsoup/soup.h>
#include <glibmm.h>
#include <memory>
#include <future>
#include <nltools/messaging/API.h>

namespace nltools
{
  namespace msg
  {
    class WebSocketJsonAPI : public API
    {
     public:
      struct ClientConnection : API::ClientConnection
      {
        ClientConnection(SoupWebsocketConnection *c);
        ~ClientConnection();
        SoupWebsocketConnection *connection;
      };

      using ReceiveCB = std::function<nlohmann::json(std::weak_ptr<ClientConnection>, const nlohmann::json &)>;

      WebSocketJsonAPI(guint port, ReceiveCB cb);
      ~WebSocketJsonAPI();

      void send(API::ClientConnection *c, const nlohmann::json &msg) override;
      void sendAll(const nlohmann::json &msg) override;
      void sendAllUpdating(const nlohmann::json &msg) override;
      bool hasClients() const override;

     private:
      using tGObjectUnref = decltype(*g_object_unref);
      using tWebSocketPtr = std::unique_ptr<SoupWebsocketConnection, tGObjectUnref>;

      void backgroundThread();
      bool doSend(SoupWebsocketConnection *c, const nlohmann::json &msg);
      static void webSocket(SoupServer *, SoupWebsocketConnection *c, const char *, SoupClientContext *,
                            WebSocketJsonAPI *pThis);
      static void receiveMessage(SoupWebsocketConnection *c, gint, GBytes *message, WebSocketJsonAPI *pThis);
      static void remove(SoupWebsocketConnection *c, WebSocketJsonAPI *pThis);

      guint m_port;
      ReceiveCB m_cb;
      std::unique_ptr<SoupServer, tGObjectUnref> m_server;
      Glib::RefPtr<Glib::MainLoop> m_messageLoop;
      std::unique_ptr<threading::ContextBoundMessageQueue> m_mainContextQueue;
      std::unique_ptr<threading::ContextBoundMessageQueue> m_bgContextQueue;
      std::list<std::shared_ptr<ClientConnection>> m_connections;
      bool m_quit = false;
      mutable std::recursive_mutex m_mutex;
      std::future<void> m_contextThread;

      nlohmann::json m_pendingUpdateMsg;
    };
  }
}
