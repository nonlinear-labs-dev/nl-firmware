#include "ContextBoundMessageQueue.h"

ContextBoundMessageQueue::ContextBoundMessageQueue(Glib::RefPtr<Glib::MainContext> context) :
        m_context(context)
{
}

void ContextBoundMessageQueue::pushMessage(tMessage&& m) {
  m_context->signal_idle().connect_once(m);
}