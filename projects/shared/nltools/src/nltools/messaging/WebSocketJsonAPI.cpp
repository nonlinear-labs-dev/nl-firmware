#include <nltools/messaging/WebSocketJsonAPI.h>
#include <nltools/logging/Log.h>

#include <utility>

namespace nltools
{
  namespace msg
  {
    WebSocketJsonAPI::ClientConnection::ClientConnection(SoupWebsocketConnection *c)
        : connection(c)
    {
    }

    WebSocketJsonAPI::ClientConnection::~ClientConnection()
    {
      g_object_unref(connection);
    }

    WebSocketJsonAPI::WebSocketJsonAPI(Glib::RefPtr<Glib::MainContext> ctx, guint port, ReceiveCB cb)
        : m_port(port)
        , m_cb(std::move(cb))
        , m_server(soup_server_new(nullptr, nullptr))
        , m_mainContext(Glib::MainContext::create())
        , m_messageLoop(Glib::MainLoop::create(m_mainContext))
        , m_mainContextQueue(std::make_unique<threading::ContextBoundMessageQueue>(ctx))
        , m_bgContextQueue(std::make_unique<threading::ContextBoundMessageQueue>(m_mainContext))
        , m_contextThread(std::async(std::launch::async, [=] { this->backgroundThread(); }))
    {
    }

    WebSocketJsonAPI::~WebSocketJsonAPI()
    {
      std::unique_lock<std::recursive_mutex> l(m_mutex);
      m_quit = true;
      m_messageLoop->quit();
      m_contextThread.wait();
      g_object_unref(m_server);
    }

    void WebSocketJsonAPI::send(API::ClientConnection *a, const nlohmann::json &msg)
    {
      m_bgContextQueue->pushMessage([this, msg, a]() {
        std::unique_lock<std::recursive_mutex> l(m_mutex);
        m_connections.remove_if(
            [this, msg, a](auto &c) { return (a == c.get()) && !this->doSend(c->connection, msg); });
      });
    }

    void WebSocketJsonAPI::sendAll(const nlohmann::json &msg)
    {
      m_bgContextQueue->pushMessage([this, msg]() {
        std::unique_lock<std::recursive_mutex> l(m_mutex);
        m_connections.remove_if([this, msg](auto &c) { return !this->doSend(c->connection, msg); });
      });
    }

    void WebSocketJsonAPI::sendAllUpdating(const nlohmann::json &msg)
    {
      std::unique_lock<std::recursive_mutex> l(m_mutex);
      m_pendingUpdateMsg = msg;

      if(!m_bgContextQueue->isPending())
        m_bgContextQueue->pushMessage([this, msg]() {
          std::unique_lock<std::recursive_mutex> l(m_mutex);
          m_connections.remove_if([this, msg](auto &c) { return !this->doSend(c->connection, m_pendingUpdateMsg); });
          m_pendingUpdateMsg.clear();
        });
    }

    bool WebSocketJsonAPI::hasClients() const
    {
      std::unique_lock<std::recursive_mutex> l(m_mutex);
      return !m_connections.empty();
    }

    void WebSocketJsonAPI::backgroundThread()
    {
      std::unique_lock<std::recursive_mutex> l(m_mutex);

      g_main_context_push_thread_default(m_messageLoop->get_context()->gobj());

      GError *error = nullptr;

      auto callback = (SoupServerWebsocketCallback) &WebSocketJsonAPI::webSocket;
      soup_server_add_websocket_handler(m_server, nullptr, nullptr, nullptr, callback, this, nullptr);
      soup_server_listen_all(m_server, m_port, SOUP_SERVER_LISTEN_IPV4_ONLY, &error);

      if(error)
      {
        Log::error(error->message);
        g_error_free(error);
      }

      if(!m_quit)
      {
        l.unlock();
        m_messageLoop->run();
      }

      g_main_context_pop_thread_default(m_messageLoop->get_context()->gobj());
    }

    bool WebSocketJsonAPI::doSend(SoupWebsocketConnection *c, const nlohmann::json &msg)
    {
      std::unique_lock<std::recursive_mutex> l(m_mutex);

      if(std::find_if(m_connections.begin(), m_connections.end(), [&](auto &a) { return c == a->connection; })
         == m_connections.end())
        return false;

      auto state = soup_websocket_connection_get_state(c);

      if(state != SOUP_WEBSOCKET_STATE_OPEN)
        return false;

      auto flattened = msg.dump();
      soup_websocket_connection_send_binary(c, flattened.c_str(), flattened.size());
      return true;
    }

    void WebSocketJsonAPI::webSocket(SoupServer *, SoupWebsocketConnection *c, const char *, SoupClientContext *,
                                     WebSocketJsonAPI *pThis)
    {
      g_signal_connect(c, "message", G_CALLBACK(&WebSocketJsonAPI::receiveMessage), pThis);
      g_signal_connect(c, "closed", G_CALLBACK(&WebSocketJsonAPI::remove), pThis);
      g_object_ref(c);
      std::unique_lock<std::recursive_mutex> l(pThis->m_mutex);
      pThis->m_connections.push_back(std::make_shared<ClientConnection>(c));
    }

    void WebSocketJsonAPI::remove(SoupWebsocketConnection *c, WebSocketJsonAPI *pThis)
    {
      pThis->m_mainContextQueue->pushMessage([pThis, c] {
        std::unique_lock<std::recursive_mutex> l(pThis->m_mutex);
        pThis->m_connections.remove_if([&](auto &e) { return c == e->connection; });
      });
    }

    void WebSocketJsonAPI::receiveMessage(SoupWebsocketConnection *c, gint, GBytes *message, WebSocketJsonAPI *pThis)
    {
      gsize length = 0;
      auto ptr = static_cast<const char *>(g_bytes_get_data(message, &length));
      try
      {
        pThis->m_mainContextQueue->pushMessage([pThis, c, j = nlohmann::json::parse(ptr, ptr + length)] {
          try
          {
            std::unique_lock<std::recursive_mutex> l(pThis->m_mutex);
            auto it = std::find_if(pThis->m_connections.begin(), pThis->m_connections.end(),
                                   [&](auto &e) { return c == e->connection; });

            if(it != pThis->m_connections.end())
              pThis->doSend(c, pThis->m_cb(*it, j));
          }
          catch(...)
          {
            nltools::Log::warning("Could not handle incoming web-socket message");
          }
        });
      }
      catch(...)
      {
        nltools::Log::warning("Could not parse incoming web-socket message");
      }
    }

  }
}
