#include "WebSocketInChannel.h"
#include <logging/Log.h>
#include <netinet/tcp.h>

namespace nltools
{
  namespace msg
  {
    namespace ws
    {

      WebSocketInChannel::WebSocketInChannel(Callback cb, guint port)
          : InChannel(cb)
          , m_port(port)
          , m_server(nullptr, g_object_unref)
          , m_contextThread(std::bind(&WebSocketInChannel::backgroundThread, this))
          , m_mainContextQueue(std::make_unique<threading::ContextBoundMessageQueue>(Glib::MainContext::get_default()))
      {
      }

      WebSocketInChannel::~WebSocketInChannel()
      {
        if(m_messageLoop)
          m_messageLoop->quit();

        if(m_contextThread.joinable())
          m_contextThread.join();
      }

      void WebSocketInChannel::backgroundThread()
      {
        auto m = Glib::MainContext::create();
        g_main_context_push_thread_default(m->gobj());

        m_server.reset(soup_server_new(nullptr, nullptr));

        GError *error = nullptr;

        auto callback = (SoupServerWebsocketCallback) &WebSocketInChannel::webSocket;
        soup_server_add_websocket_handler(m_server.get(), nullptr, nullptr, nullptr, callback, this, nullptr);
        soup_server_listen_all(m_server.get(), m_port, SOUP_SERVER_LISTEN_IPV4_ONLY, &error);

        if(error)
        {
          nltools::Log::error(error->message);
          g_error_free(error);
        }

        m_messageLoop = Glib::MainLoop::create(m);
        m_messageLoop->run();
      }

      void WebSocketInChannel::webSocket(SoupServer *, SoupWebsocketConnection *c, const char *, SoupClientContext *,
                                         WebSocketInChannel *pThis)
      {
        g_signal_connect(c, "message", G_CALLBACK(&WebSocketInChannel::receiveMessage), pThis);
        g_object_ref(c);
        g_object_set(c, "keepalive-interval", 5, nullptr);
        pThis->m_connections.push_back(tWebSocketPtr(c, g_object_unref));
      }

      void WebSocketInChannel::receiveMessage(SoupWebsocketConnection *, gint, GBytes *message,
                                              WebSocketInChannel *pThis)
      {
        auto bytes = Glib::wrap(g_bytes_ref(message));
        pThis->m_mainContextQueue->pushMessage([=] { pThis->onMessageReceived(bytes); });
      }
    }
  }
}
