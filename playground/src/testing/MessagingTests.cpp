#include <messaging/Messaging.h>
#include <messaging/Message.h>
#include <logging/Log.h>
#include "TestDriver.h"
#include <glib.h>
#include <tools/Expiration.h>

struct MessagingTests
{
  static void registerTests()
  {
    using namespace std::chrono_literals;

    g_test_add_func("/Messaging/send-receive", [] {
      using namespace nltools::msg;
      int numReceivedMessages = 0;
      auto parameterChangedConnection
          = receive<MessageType::Parameter, Receivers::Playground>([&](const auto &msg) { numReceivedMessages++; });

      auto presetChangedConnectionc = receive<MessageType::Preset, Receivers::Playground>(
          [&](const auto &msg) { nltools::notify("Got preset message from audioEngine!"); });

      ParameterChangedMessage msgToSend;

      waitForConnection(Receivers::AudioEngine);
      send(Receivers::AudioEngine, msgToSend);

      Expiration exp;
      exp.refresh(12s);

      while(exp.isPending() && numReceivedMessages == 0)
        g_main_context_iteration(nullptr, TRUE);

      g_assert(numReceivedMessages == 1);
    });
  }
};

static TestDriver<MessagingTests> driver;
