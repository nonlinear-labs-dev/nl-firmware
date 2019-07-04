#pragma once

#include <memory>
#include <giomm.h>
#include <sigc++/sigc++.h>
#include <assert.h>

namespace nltools
{
  namespace msg
  {
    // forward declaration of message types:
    struct ParameterChangedMessage;
    struct PresetChangedMessage;

    // known ports for paritcipants:
    enum Ports
    {
      FirstPort = 40100,

      AudioEngineWebSocket,
      PlaygroundWebSocket,
    };

    // Types:
    using SerializedMessage = Glib::RefPtr<Glib::Bytes>;

    enum class Participants
    {
      Lpc,
      Oled,
      PanelLed,
      RibbonLed,
      TimeStampedOled,
      AudioEngine,
      Playground
    };

    enum class MessageType : uint16_t
    {
      Preset,
      Morph_A,
      Morph_B,
      Parameter,
      EditControl,
      MorphPosition,
      Setting,
      Notification,
      Assertion,
      Request
    };

    namespace detail
    {
      // default (de)serialization for messages, may be specialized for more compilcated types:
      template <typename Msg> Msg deserialize(const SerializedMessage &s)
      {
        Msg ret;
        assert(s->get_size() == sizeof(Msg));
        gsize numBytes = 0;
        memcpy(&ret, s->get_data(numBytes), sizeof(Msg));
        return ret;
      }

      template <typename Msg> SerializedMessage serialize(const Msg &msg)
      {
        return Glib::Bytes::create(&msg, sizeof(Msg));
      }

      // send raw bytes to receiver
      void send(Participants receiver, SerializedMessage msg);

      template <typename Msg> sigc::connection receive(MessageType type, std::function<void(const Msg &)> cb)
      {
        return receiveSerialized(type, [=](const SerializedMessage &s) { cb(detail::deserialize<Msg>(s)); });
      }

      sigc::connection receiveSerialized(MessageType type, std::function<void(const SerializedMessage &)> cb);
    }

    // Client has to call this on startup

    struct ChannelConfiguration
    {
      Participants peer;
      std::string uri;
    };

    struct Configuration
    {
      ChannelConfiguration inChannel;
      std::vector<ChannelConfiguration> outChannels;
    };

    void init(const Configuration &conf);

    // wait at most timeOut for the connection to be established
    // return true if there is a connection to receiver
    bool waitForConnection(Participants receiver, std::chrono::milliseconds timeOut = std::chrono::seconds(10));

    // Send msg to receiver. If there is no receiver, does nothing.
    template <typename Msg> void send(Participants receiver, const Msg &msg)
    {
      detail::send(receiver, detail::serialize<Msg>(msg));
    }

    template <typename M> sigc::connection receive(std::function<void(const M &)> cb)
    {
      return detail::receive<M>(M::theType, [=](const auto &s) { cb(s); });
    }
  }
}
