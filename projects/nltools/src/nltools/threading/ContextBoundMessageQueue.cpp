#include <nltools/threading/ContextBoundMessageQueue.h>
#include <nltools/logging/Log.h>
#include <glibmm.h>

namespace nltools
{
  namespace threading
  {
    ContextBoundMessageQueue::ContextBoundMessageQueue(Glib::RefPtr<Glib::MainContext> context)
        : m_context(std::move(context))
    {
    }

    ContextBoundMessageQueue::~ContextBoundMessageQueue() = default;

    void ContextBoundMessageQueue::pushMessage(tMessage &&m)
    {
      m_context->invoke([m = std::move(m)]() {
        m();
        return false;
      });
    }
  }
}
