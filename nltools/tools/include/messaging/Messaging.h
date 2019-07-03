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
    class ParameterChangedMessage;
    class PresetChangedMessage;

    // known ports for paritcipants:
    enum Ports
    {
      FirstPort = 40100,

      AudioEngineWebSocket,
      PlaygroundWebSocket,
    };

    // Types:
    using SerializedMessage = Glib::RefPtr<Glib::Bytes>;

    enum class Receivers
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
      void send(Receivers receiver, SerializedMessage msg);

      template <typename Msg>
      sigc::connection receive(Receivers receiver, MessageType type, std::function<void(const Msg &)> cb)
      {
        return receive(receiver, type, [=](const SerializedMessage &s) { cb(detail::deserialize<Msg>(s)); });
      }

      sigc::connection receive(Receivers receiver, MessageType type, std::function<void(const SerializedMessage &)> cb);

      // map ParameterType to Parameter-class
      template <MessageType type> struct MessageTypeMap
      {
      };

      template <> struct MessageTypeMap<MessageType::Parameter>
      {
        using Message = ParameterChangedMessage;
      };

      template <> struct MessageTypeMap<MessageType::Preset>
      {
        using Message = PresetChangedMessage;
      };
    }

    // wait at most timeOut for the connection to be established
    // return true if there is a connection to receiver
    bool waitForConnection(Receivers receiver, std::chrono::milliseconds timeOut = std::chrono::seconds(10));

    // Send msg to receiver. If there is no receiver, does nothing.
    template <typename Msg> void send(Receivers receiver, const Msg &msg)
    {
      detail::send(receiver, detail::serialize<Msg>(msg));
    }

    // receive messages of type in cb
    template <MessageType type, Receivers receiver>
    sigc::connection receive(std::function<void(const typename detail::MessageTypeMap<type>::Message &)> cb)
    {
      return detail::receive<typename detail::MessageTypeMap<type>::Message>(receiver, type,
                                                                             [=](const auto &s) { cb(s); });
    }
  }
}
