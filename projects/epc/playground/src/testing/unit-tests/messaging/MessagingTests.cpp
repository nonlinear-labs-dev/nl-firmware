#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <nltools/logging/Log.h>
#include <glibmm.h>
#include <nltools/threading/Expiration.h>
#include <nltools/threading/ContextBoundMessageQueue.h>
#include <testing/TestHelper.h>

using namespace nltools::msg;
using namespace std::chrono_literals;

TEST_CASE_METHOD(TestHelper::MainContextFixture, "Notify on discovery", "[Messaging][nltools]")
{
  TestHelper::ScopedMessagingConfiguration scopeEndPoint {
    { { EndPoint::TestEndPoint }, { EndPoint::TestEndPoint }, m_context }
  };

  bool received = false;
  auto c = onConnectionEstablished(EndPoint::TestEndPoint, [&] { received = true; });
  TestHelper::doMainLoop(
      10ms, 1s, [&] { return received; }, m_context);
  c.disconnect();
}

TEST_CASE_METHOD(TestHelper::MainContextFixture, "Send Receive", "[Messaging][nltools]")
{
  REQUIRE(g_main_context_default() != nullptr);
  TestHelper::ScopedMessagingConfiguration scopeEndPoint {
    { { EndPoint::TestEndPoint }, { EndPoint::TestEndPoint }, m_context }
  };

  int numMessages = 0;

  SplitPresetMessage msgToSend {};
  CHECK(waitForConnection(EndPoint::TestEndPoint));
  auto c = receive<SplitPresetMessage>(EndPoint::TestEndPoint, [&](const auto &msg) { numMessages++; });
  send(EndPoint::TestEndPoint, msgToSend);
  TestHelper::doMainLoop(
      10ms, 1s, [&] { return numMessages == 1; }, m_context);
  c.disconnect();
}

TEST_CASE_METHOD(TestHelper::MainContextFixture, "No packet lost if bombed", "[Messaging][nltools]")
{
  REQUIRE(g_main_context_default() != nullptr);
  TestHelper::ScopedMessagingConfiguration scopeEndPoint {
    { { EndPoint::TestEndPoint }, { EndPoint::TestEndPoint }, m_context }
  };

  int numRecMessages = 0;
  int numSendMessages = 1000;

  SplitPresetMessage msgToSend {};
  CHECK(waitForConnection(EndPoint::TestEndPoint));
  auto c = receive<SplitPresetMessage>(EndPoint::TestEndPoint, [&](const auto &msg) { numRecMessages++; });

  for(int i = 0; i < numSendMessages; i++)
    send(EndPoint::TestEndPoint, msgToSend);

  TestHelper::doMainLoop(
      10ms, 1s, [&] { return numRecMessages == numSendMessages; }, m_context);
  c.disconnect();
}

TEST_CASE_METHOD(TestHelper::MainContextFixture, "No packet doubles", "[Messaging][nltools]")
{
  REQUIRE(g_main_context_default() != nullptr);
  TestHelper::ScopedMessagingConfiguration scopeEndPoint {
    { { EndPoint::TestEndPoint }, { EndPoint::TestEndPoint }, m_context }
  };

  int numRecMessages = 0;
  int numSendMessages = 100;

  SplitPresetMessage msgToSend {};
  CHECK(waitForConnection(EndPoint::TestEndPoint));
  auto c = receive<SplitPresetMessage>(EndPoint::TestEndPoint, [&](const auto &msg) { numRecMessages++; });

  for(int i = 0; i < numSendMessages; i++)
    send(EndPoint::TestEndPoint, msgToSend);

  TestHelper::doMainLoop(
      10ms, 1s, [&] { return numRecMessages <= numSendMessages; }, m_context);
  c.disconnect();
}

TEST_CASE("ContextBoundMessageQueue - no send after destruction")
{
  auto ctx = Glib::MainContext::create();
  auto queue = std::make_unique<nltools::threading::ContextBoundMessageQueue>(ctx);
  bool firstCBReached = false;
  bool secondCBReached = false;
  queue->pushMessage([&] { firstCBReached = true; });
  ctx->iteration(FALSE);
  queue->pushMessage([&] { secondCBReached = true; });
  queue.reset();
  ctx->iteration(FALSE);

  CHECK(firstCBReached);
  CHECK(!secondCBReached);
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Send all Parameter Messages")
{
  int testedParams = 0;
  for(auto vg : { VoiceGroup::Global, VoiceGroup::I, VoiceGroup::II })
  {
    for(auto g : app->getPresetManager()->getEditBuffer()->getParameterGroups(vg))
    {
      for(auto &p : g->getParameters())
      {
        testedParams++;
        REQUIRE_NOTHROW(p->sendParameterMessage());
      }
    }
  }

  REQUIRE(testedParams > 0);
}