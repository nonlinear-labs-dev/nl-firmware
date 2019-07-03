#include <threading/ContextBoundMessageQueue.h>

namespace nltools
{
  namespace threading
  {
    ContextBoundMessageQueue::ContextBoundMessageQueue(Glib::RefPtr<Glib::MainContext> context)
        : m_context(context)
    {
    }

    void ContextBoundMessageQueue::pushMessage(tMessage&& m)
    {
      std::lock_guard<std::recursive_mutex> l(m_mutex);
      m_queue.push_back(std::move(m));
      g_main_context_invoke(m_context->gobj(), (GSourceFunc) &ContextBoundMessageQueue::doQueuedStuff, this);
    }

    gboolean ContextBoundMessageQueue::doQueuedStuff(ContextBoundMessageQueue* pThis)
    {
      std::lock_guard<std::recursive_mutex> l(pThis->m_mutex);

      if(!pThis->m_queue.empty())
      {
        auto& m = pThis->m_queue.front();
        m();
        pThis->m_queue.pop_front();
      }

      return !pThis->m_queue.empty();
    }
  }
}
