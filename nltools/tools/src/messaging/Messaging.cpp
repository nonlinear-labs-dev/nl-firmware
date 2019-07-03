#include <messaging/Messaging.h>
#include <messaging/websocket/WebSocketOutChannel.h>
#include <messaging/websocket/WebSocketInChannel.h>
#include <logging/Log.h>
#include <memory>
#include <map>
#include <list>

namespace nltools
{
  namespace msg
  {
    namespace detail
    {
      using OutChannels = std::map<Receivers, std::unique_ptr<OutChannel>>;
      using InChannels = std::list<std::unique_ptr<InChannel>>;
      using Signals = std::map<MessageType, sigc::signal<void, const SerializedMessage &>>;

      static OutChannels createOutChannels()
      {
        OutChannels ret;
        ret[Receivers::AudioEngine]
            = std::make_unique<ws::WebSocketOutChannel>("localhost", Ports::AudioEngineWebSocket);
        ret[Receivers::Playground] = std::make_unique<ws::WebSocketOutChannel>("localhost", Ports::PlaygroundWebSocket);
        return ret;
      }

      static InChannels createInChannels(Receivers receiver, std::function<void(const SerializedMessage &)> cb)
      {
        InChannels ret;

        switch(receiver)
        {
          case Receivers::AudioEngine:
            ret.push_back(std::make_unique<ws::WebSocketInChannel>(cb, Ports::AudioEngineWebSocket));
            break;

          case Receivers::Playground:
            ret.push_back(std::make_unique<ws::WebSocketInChannel>(cb, Ports::PlaygroundWebSocket));
            break;

          default:
            nltools::error("Cannot create input channel!");
        }

        return ret;
      }

      static OutChannel *getOutChannel(Receivers r)
      {
        static OutChannels outChannels = createOutChannels();
        return outChannels.at(r).get();
      }

      static Signals &getSignals()
      {
        static Signals signals;
        return signals;
      }

      static sigc::connection connectReceiver(MessageType type, std::function<void(const SerializedMessage &)> cb)
      {
        return getSignals()[type].connect(cb);
      }

      static void notifyClients(const SerializedMessage &s)
      {
        gsize numBytes = 0;
        auto data = reinterpret_cast<const uint16_t *>(s->get_data(numBytes));
        auto type = static_cast<MessageType>(data[0]);
        getSignals()[type](s);
      }

      void send(nltools::msg::Receivers receiver, SerializedMessage msg)
      {
        getOutChannel(receiver)->send(msg);
      }

      sigc::connection receive(Receivers receiver, MessageType type, std::function<void(const SerializedMessage &)> cb)
      {
        static InChannels inChannels = createInChannels(receiver, [](const auto &s) { notifyClients(s); });
        return connectReceiver(type, cb);
      }
    }

    bool waitForConnection(Receivers receiver, std::chrono::milliseconds timeOut)
    {
      return detail::getOutChannel(receiver)->waitForConnection(timeOut);
    }
  }
}
