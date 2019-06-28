#pragma once

#include <memory>
#include <sigc++/sigc++.h>

namespace nltools
{
  namespace msg
  {
    enum class Senders
    {
      Lpc,
      Buttons,
      Rotary,
      TimeStampedRotary,
      AudioEngine,
      Playground
    };

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

    enum class MessageType
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

    class Message;

    class Messaging
    {
     public:
      using tMessage = std::unique_ptr<Message>;
      using Callback = std::function<void(const Message *)>;

      static void send(Senders sender, Receivers receiver, tMessage msg);
      static sigc::connection receive(Senders sender, Receivers receiver, Callback cb);
    };
  }
}
