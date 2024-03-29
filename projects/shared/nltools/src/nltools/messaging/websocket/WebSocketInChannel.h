#pragma once

#include <nltools/GlibFwd.h>
#include <nltools/messaging/InChannel.h>
#include <nltools/threading/BackgroundThreadWaiter.h>
#include <nltools/threading/Threading.h>
#include <libsoup/soup.h>
#include <memory>
#include <list>
#include <future>
#include <glibmm/refptr.h>

namespace nltools
{
  namespace threading
  {
    class ContextBoundMessageQueue;

  }
  namespace msg
  {
    namespace ws
    {
      class WebSocketInChannel : public InChannel
      {
       public:
        WebSocketInChannel(Callback cb, guint port, nltools::threading::Priority p,
                           const Glib::RefPtr<Glib::MainContext> &ctx);
        ~WebSocketInChannel() override;

       private:
        void backgroundThread(nltools::threading::Priority p);

        static void webSocket(SoupServer *server, SoupWebsocketConnection *connection, const char *pathStr,
                              SoupClientContext *client, WebSocketInChannel *pThis);
        static void receiveMessage(SoupWebsocketConnection *, gint, GBytes *message, WebSocketInChannel *pThis);

        using tWebSocketPtr = std::unique_ptr<SoupWebsocketConnection, decltype(*g_object_unref)>;

        guint m_port;
        std::unique_ptr<SoupServer, decltype(*g_object_unref)> m_server;
        Glib::RefPtr<Glib::MainContext> m_backgroundCtx;
        Glib::RefPtr<Glib::MainLoop> m_backgroundLoop;
        BackgroundThreadWaiter m_conditionEstablishedThreadWaiter;
        std::future<void> m_backgroundTask;

        std::unique_ptr<threading::ContextBoundMessageQueue> m_mainContextQueue;
        std::list<tWebSocketPtr> m_connections;
      };
    }
  }
}
