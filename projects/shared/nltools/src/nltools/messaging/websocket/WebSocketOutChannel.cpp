#include "WebSocketOutChannel.h"
#include <nltools/StringTools.h>
#include <nltools/logging/Log.h>
#include <netinet/tcp.h>
#include <glibmm.h>
#include <giomm.h>
#include <nltools/messaging/Message.h>
#include <pthread.h>
#include <thread>

namespace nltools
{
  namespace msg
  {
    namespace ws
    {
      WebSocketOutChannel::WebSocketOutChannel(const std::string &targetMachine, guint port,
                                               nltools::threading::Priority p,
                                               std::function<void()> connectionEstablishedCB,
                                               Glib::RefPtr<Glib::MainContext> ctx)
          : m_cancel(Gio::Cancellable::create())
          , m_uri(nltools::string::concat("http://", targetMachine, ":", port))
          , m_soupSession(soup_session_new(), g_object_unref)
          , m_connection(nullptr, g_object_unref)
          , m_mainThreadContextQueue(std::make_unique<threading::ContextBoundMessageQueue>(ctx))
          , m_onConnectionEstablished(connectionEstablishedCB)
      {
        connect();
        ctx->signal_timeout().connect_seconds(sigc::mem_fun(this, &WebSocketOutChannel::ping), 2);
      }

      WebSocketOutChannel::~WebSocketOutChannel()
      {
        m_cancel->cancel();
      }

      bool WebSocketOutChannel::connect()
      {
        auto msg = soup_message_new("GET", m_uri.c_str());
        auto cb = reinterpret_cast<GAsyncReadyCallback>(&WebSocketOutChannel::onWebSocketConnected);
        soup_session_websocket_connect_async(m_soupSession.get(), msg, nullptr, nullptr, m_cancel->gobj(), cb, this);
        g_object_unref(msg);
        return false;
      }

      void WebSocketOutChannel::onWebSocketConnected(SoupSession *session, GAsyncResult *res,
                                                     WebSocketOutChannel *pThis)
      {
        GError *error = nullptr;

        if(SoupWebsocketConnection *connection = soup_session_websocket_connect_finish(session, res, &error))
          pThis->connectWebSocket(connection);

        if(error)
        {
          nltools::Log::warning(__PRETTY_FUNCTION__, __LINE__, error->code);
          nltools::Log::debug(pThis->m_uri, " -> ", error->message);

          if(error->code == Gio::Error::Code::CANCELLED)
          {
            g_error_free(error);
            return;
          }

          pThis->reconnect();
          g_error_free(error);
        }
        else
        {
          pThis->signalConnectionEstablished();
        }
      }

      bool WebSocketOutChannel::send(const SerializedMessage &msg)
      {
        if(!m_connection || m_flushing)
          return false;

        auto state = soup_websocket_connection_get_state(m_connection.get());

        if(state == SOUP_WEBSOCKET_STATE_OPEN)
        {
          gsize len = 0;
          auto data = msg->get_data(len);
          soup_websocket_connection_send_binary(m_connection.get(), data, len);
          return true;
        }
        else
        {
          m_connection.reset();
          reconnect();
          return false;
        }
      }

      void WebSocketOutChannel::flush(std::chrono::milliseconds timeout)
      {
#warning "todo"
      }

      bool WebSocketOutChannel::waitForConnection(std::chrono::milliseconds timeOut)
      {
        return m_connectionEstablishedWaiter.waitFor(timeOut);
      }

      bool WebSocketOutChannel::isConnected() const
      {
        return m_connectionEstablishedWaiter.isNotified();
      }

      bool WebSocketOutChannel::ping()
      {
        send(detail::serialize(PingMessage()));
        return true;
      }

      void WebSocketOutChannel::signalConnectionEstablished()
      {
        nltools::Log::warning(__PRETTY_FUNCTION__, __LINE__);
        m_connectionEstablishedWaiter.notify();
        m_mainThreadContextQueue->pushMessage([this] { this->m_onConnectionEstablished(); });
      }

      void WebSocketOutChannel::reconnect()
      {
        nltools::Log::warning(__PRETTY_FUNCTION__, __LINE__);
        m_reconnect = m_mainThreadContextQueue->getContext()->signal_timeout().connect_seconds(
            sigc::mem_fun(this, &WebSocketOutChannel::connect), 2);
      }

      void WebSocketOutChannel::connectWebSocket(SoupWebsocketConnection *connection)
      {
        auto stream = soup_websocket_connection_get_io_stream(connection);
        auto outStream = g_io_stream_get_output_stream(stream);

        GError *error = nullptr;
        GSocket *socket = nullptr;
        g_object_get(outStream, "socket", &socket, nullptr);

        auto ret = g_socket_set_option(socket, SOL_TCP, TCP_NODELAY, 1, &error);

        if(error)
        {
          nltools::Log::error(error->message);
          g_error_free(error);
        }

        if(!ret)
          nltools::Log::warning("setting socket option NODELAY failed");

        m_connection.reset(connection);
      }
    }
  }
}
