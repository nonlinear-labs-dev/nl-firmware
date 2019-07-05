#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <nltools/logging/Log.h>
#include "TestDriver.h"
#include <glib.h>
#include <nltools/threading/Expiration.h>

struct MessagingTests
{
  static void registerTests()
  {
    using namespace std::chrono_literals;

    g_test_add_func("/Messaging/send-receive", [] {
      using namespace nltools::msg;
      int numReceivedMessages = 0;

      ParameterChangedMessage msgToSend(12, 0.3);
      nltools::Log::notify("waiting for connection to playground endpoint");
      assert(waitForConnection(EndPoint::Playground));

      auto c = receive<ParameterChangedMessage>(EndPoint::Playground, [&](const auto &msg) { numReceivedMessages++; });

      send(EndPoint::Playground, msgToSend);

      Expiration exp;
      exp.refresh(2s);

      while(exp.isPending() && numReceivedMessages < 1)
      {
        nltools::Log::notify("waiting for receiving the message");
        g_main_context_iteration(nullptr, TRUE);
      }

      assert(numReceivedMessages == 1);
    });

    g_test_add_func("/Messaging/no-packets-lost-if-bombed", [] {
      using namespace nltools::msg;
      int numReceivedMessages = 0;
      int numSendMessages = 10;

      ParameterChangedMessage msgToSend(12, 0.3);
      nltools::Log::notify("waiting for connection to playground endpoint");
      assert(waitForConnection(EndPoint::Playground));

      auto c = receive<ParameterChangedMessage>(EndPoint::Playground, [&](const auto &msg) { numReceivedMessages++; });

      for(int i = 0; i < numSendMessages; i++)
        send(EndPoint::Playground, msgToSend);

      Expiration exp;
      exp.refresh(60s);

      nltools::Log::notify("waiting for receiving the messages");
      while(exp.isPending() && numReceivedMessages < numSendMessages)
        g_main_context_iteration(nullptr, TRUE);

      nltools::Log::notify("received all messages");
      assert(numReceivedMessages == numSendMessages);
    });
  }
};

static TestDriver<MessagingTests> driver;
