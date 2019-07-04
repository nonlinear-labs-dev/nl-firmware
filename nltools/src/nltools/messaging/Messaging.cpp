#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/websocket/WebSocketOutChannel.h>
#include <nltools/messaging/websocket/WebSocketInChannel.h>
#include <nltools/logging/Log.h>
#include <nltools/StringTools.h>
#include <memory>
#include <map>
#include <list>

namespace nltools
{
  namespace msg
  {
    namespace detail
    {
      using OutChannels = std::map<Participants, std::unique_ptr<OutChannel>>;
      using InChannelPtr = std::unique_ptr<InChannel>;
      using Signals = std::map<MessageType, sigc::signal<void, const SerializedMessage &>>;

      static OutChannels outChannels;
      static InChannelPtr inChannel;
      static Signals signals;

      static void notifyClients(const SerializedMessage &s)
      {
        gsize numBytes = 0;
        auto data = reinterpret_cast<const uint16_t *>(s->get_data(numBytes));
        auto type = static_cast<MessageType>(data[0]);
        signals[type](s);
      }

      static void createInChannel(const Configuration &conf)
      {
        auto cb = [](const auto &s) { notifyClients(s); };
        parseURI(conf.inChannel.uri, [=](auto scheme, auto, auto, auto port) {
          assert(scheme == "ws");  // Currently, only web sockets are supported
          inChannel = std::make_unique<ws::WebSocketInChannel>(cb, port);
        });
      }

      static void createOutChannels(const Configuration &conf)
      {
        for(const auto &c : conf.outChannels)
        {
          parseURI(c.uri, [=](auto scheme, auto host, auto, auto port) {
            assert(scheme == "ws");  // Currently, only web sockets are supported
            outChannels[c.peer] = std::make_unique<ws::WebSocketOutChannel>(host, port);
          });
        }
      }

      static sigc::connection connectReceiver(MessageType type, std::function<void(const SerializedMessage &)> cb)
      {
        return signals[type].connect(cb);
      }

      void send(nltools::msg::Participants receiver, SerializedMessage msg)
      {
        outChannels.at(receiver)->send(msg);
      }

      sigc::connection receiveSerialized(MessageType type, std::function<void(const SerializedMessage &)> cb)
      {
        return connectReceiver(type, cb);
      }
    }

    bool waitForConnection(Participants receiver, std::chrono::milliseconds timeOut)
    {
      return detail::outChannels.at(receiver)->waitForConnection(timeOut);
    }

    void init(const Configuration &conf)
    {
      detail::createInChannel(conf);
      detail::createOutChannels(conf);
    }
  }
}
