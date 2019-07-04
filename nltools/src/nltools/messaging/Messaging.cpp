#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/websocket/WebSocketOutChannel.h>
#include <nltools/messaging/websocket/WebSocketInChannel.h>
#include <nltools/logging/Log.h>
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
      using Signals = std::map<MessageType, sigc::signal<void, const SerializedMessage &>>;

      static OutChannels createOutChannels()
      {
        OutChannels ret;
        ret[Participants::AudioEngine]
            = std::make_unique<ws::WebSocketOutChannel>("localhost", Ports::AudioEngineWebSocket);
        ret[Participants::Playground]
            = std::make_unique<ws::WebSocketOutChannel>("localhost", Ports::PlaygroundWebSocket);
        return ret;
      }

      static Signals &getSignals()
      {
        static Signals signals;
        return signals;
      }

      static void notifyClients(const SerializedMessage &s)
      {
        gsize numBytes = 0;
        auto data = reinterpret_cast<const uint16_t *>(s->get_data(numBytes));
        auto type = static_cast<MessageType>(data[0]);
        getSignals()[type](s);
      }

      static std::unique_ptr<InChannel> createInChannel(Participants whoAmI)
      {
        auto cb = [](const auto &s) { notifyClients(s); };

        switch(whoAmI)
        {
          case Participants::AudioEngine:
            return std::make_unique<ws::WebSocketInChannel>(cb, Ports::AudioEngineWebSocket);

          case Participants::Playground:
            return std::make_unique<ws::WebSocketInChannel>(cb, Ports::PlaygroundWebSocket);

          default:
            nltools::Log::error("Cannot create input channel!");
        }

        return nullptr;
      }

      static OutChannel *getOutChannel(Participants r)
      {
        static OutChannels outChannels = createOutChannels();
        return outChannels.at(r).get();
      }

      static sigc::connection connectReceiver(MessageType type, std::function<void(const SerializedMessage &)> cb)
      {
        return getSignals()[type].connect(cb);
      }

      void send(nltools::msg::Participants receiver, SerializedMessage msg)
      {
        getOutChannel(receiver)->send(msg);
      }

      sigc::connection receiveSerialized(MessageType type, std::function<void(const SerializedMessage &)> cb)
      {
        return connectReceiver(type, cb);
      }
    }

    bool waitForConnection(Participants receiver, std::chrono::milliseconds timeOut)
    {
      return detail::getOutChannel(receiver)->waitForConnection(timeOut);
    }

    void init(Participants self)
    {
      static auto inChannel = detail::createInChannel(self);
    }
  }
}
