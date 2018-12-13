#pragma once

#include <bbbb.h>
#include <libsoup/soup.h>
#include <memory>
#include <list>
#include <functional>

class WebSocketServer
{
 public:
  WebSocketServer();
  virtual ~WebSocketServer();

  using tMessage = Glib::RefPtr<Glib::Bytes>;

  void sendMessage(tMessage msg);
  sigc::connection onMessageReceived(Domain domain, const sigc::slot<void, tMessage> &cb);

 private:
  void connectWebSocket(SoupWebsocketConnection *connection);
  static void webSocket(SoupServer *server, SoupWebsocketConnection *connection, const char *pathStr,
                        SoupClientContext *client, WebSocketServer *pThis);
  static void receiveMessage(SoupWebsocketConnection *, gint, GBytes *message, WebSocketServer *pThis);

  using tWebSocketPtr = std::unique_ptr<SoupWebsocketConnection, decltype(*g_object_unref)>;
  using tSignal = sigc::signal<void, tMessage>;

  std::unique_ptr<SoupServer, decltype(*g_object_unref)> m_server;
  std::list<tWebSocketPtr> m_connections;

  std::map<Domain, tSignal> m_onMessageReceived;
};
