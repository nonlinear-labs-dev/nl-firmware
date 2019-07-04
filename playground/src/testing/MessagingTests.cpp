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
      assert(waitForConnection(Participants::Playground));

      auto c = receive<ParameterChangedMessage>([&](const auto &msg) { numReceivedMessages++; });

      send(Participants::Playground, msgToSend);

      Expiration exp;
      exp.refresh(2s);

      while(exp.isPending() && numReceivedMessages < 1)
        g_main_context_iteration(nullptr, TRUE);

      assert(numReceivedMessages == 1);
    });

    g_test_add_func("/Messaging/no-packets-lost-if-bombed", [] {
      using namespace nltools::msg;
      int numReceivedMessages = 0;
      int numSendMessages = 10000;

      ParameterChangedMessage msgToSend(12, 0.3);
      assert(waitForConnection(Participants::Playground));

      auto c = receive<ParameterChangedMessage>([&](const auto &msg) { numReceivedMessages++; });

      for(int i = 0; i < numSendMessages; i++)
        send(Participants::Playground, msgToSend);

      Expiration exp;
      exp.refresh(60s);

      while(exp.isPending() && numReceivedMessages < numSendMessages)
        g_main_context_iteration(nullptr, TRUE);

      assert(numReceivedMessages == numSendMessages);
    });
  }
};

static TestDriver<MessagingTests> driver;
