#include "ContextBoundMessageQueue.h"

ContextBoundMessageQueue::ContextBoundMessageQueue(Glib::RefPtr<Glib::MainContext> context) : m_context(context) {
}

void ContextBoundMessageQueue::pushMessage(tMessage m) {
  auto idle = m_context->signal_idle();
  idle.connect_once(m);
}