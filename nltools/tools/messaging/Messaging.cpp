#include "Messaging.h"
#include "MessageComposer.h"

void intendedUsage_Sending()
{
  using namespace nltools::msg;

  // sending a message
  MessageComposer composer(Senders::AudioEngine, Receivers::Playground, MessageType::Parameter);
  composer.add(static_cast<uint16_t>(12));
  composer.add(1.23f);
  composer.send();
}

void intendedUsage_Receiving()
{
  using namespace nltools::msg;

  // receiving messages
  auto connection = receive(Senders::Playground, Receivers::AudioEngine, [](auto msg) {
    // do fancy stuff with msg
  });

  connection.disconnect();
  // no messages will be received anymore
}

void intendedUsage_Receiving_OO()
{
  using namespace nltools::msg;

  class FooBar : public sigc::trackable
  {
    FooBar()
    {
      receive(Senders::Playground, Receivers::AudioEngine, sigc::mem_fun(this, &FooBar::onMessage));
    }

    void onMessage(const Message *msg)
    {
      // do fancy stuff with msg
    }
  };
}
