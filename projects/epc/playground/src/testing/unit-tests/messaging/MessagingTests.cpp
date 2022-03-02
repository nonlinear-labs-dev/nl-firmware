#include <nltools/messaging/Messaging.h>
#include <nltools/messaging/Message.h>
#include <nltools/logging/Log.h>
#include <glibmm.h>
#include <nltools/threading/Expiration.h>
#include <nltools/threading/ContextBoundMessageQueue.h>
#include <testing/TestHelper.h>

using namespace nltools::msg;
using namespace std::chrono_literals;

#warning "This Test is unreliable and leads to hung playground-test process if unlucky"
TEST_CASE("Notify on discovery", "[Messaging][nltools]")
{
  TestHelper::ScopedMessagingConfiguration scopeEndPoint { {{EndPoint::TestEndPoint}, {EndPoint::TestEndPoint}} };

  bool received = false;
  auto c = onConnectionEstablished(EndPoint::TestEndPoint, [&] { received = true; });
  TestHelper::doMainLoop(1s, 1s, [&] { return received; });
  c.disconnect();
}

TEST_CASE("Send Receive", "[Messaging][nltools]")
{
  TestHelper::ScopedMessagingConfiguration scopeEndPoint { {{EndPoint::TestEndPoint}, {EndPoint::TestEndPoint}} };

  int numMessages = 0;
  UnmodulateableParameterChangedMessage msgToSend { 12, 0.3, VoiceGroup::I };
  CHECK(waitForConnection(EndPoint::TestEndPoint));
  auto c
      = receive<UnmodulateableParameterChangedMessage>(EndPoint::TestEndPoint, [&](const auto &msg) { numMessages++; });
  send(EndPoint::TestEndPoint, msgToSend);
  TestHelper::doMainLoop(1s, 1s, [&] { return numMessages == 1; });
  c.disconnect();
}

#warning "unreliable test-case I aborted after 7 Minutes"
TEST_CASE("No packet lost if bombed", "[Messaging][nltools]")
{
  TestHelper::ScopedMessagingConfiguration scopeEndPoint { {{EndPoint::TestEndPoint}, {EndPoint::TestEndPoint}} };

  int numRecMessages = 0;
  int numSendMessages = 1000;

  UnmodulateableParameterChangedMessage msgToSend { 12, 0.3, VoiceGroup::I };
  CHECK(waitForConnection(EndPoint::TestEndPoint));
  auto c = receive<UnmodulateableParameterChangedMessage>(EndPoint::TestEndPoint,
                                                          [&](const auto &msg) { numRecMessages++; });

  for(int i = 0; i < numSendMessages; i++)
    send(EndPoint::TestEndPoint, msgToSend);

  TestHelper::doMainLoop(1s, 1s, [&] { return numRecMessages == numSendMessages; });
  c.disconnect();
}

TEST_CASE("No packet doubles", "[Messaging][nltools]")
{
  TestHelper::ScopedMessagingConfiguration scopeEndPoint { {{EndPoint::TestEndPoint}, {EndPoint::TestEndPoint}} };

  int numRecMessages = 0;
  int numSendMessages = 100;

  UnmodulateableParameterChangedMessage msgToSend { 12, 0.3, VoiceGroup::I };
  CHECK(waitForConnection(EndPoint::TestEndPoint));
  auto c = receive<UnmodulateableParameterChangedMessage>(EndPoint::TestEndPoint,
                                                          [&](const auto &msg) { numRecMessages++; });

  for(int i = 0; i < numSendMessages; i++)
    send(EndPoint::TestEndPoint, msgToSend);

  TestHelper::doMainLoop(1s, 1s, [&] { return numRecMessages <= numSendMessages; });
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