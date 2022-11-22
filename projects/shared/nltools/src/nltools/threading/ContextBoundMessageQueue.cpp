#include <nltools/threading/ContextBoundMessageQueue.h>
#include <glibmm.h>
#include <thread>
#include "nltools/logging/Log.h"

using namespace std::chrono_literals;

namespace nltools
{
  namespace threading
  {
    ContextBoundMessageQueue::ContextBoundMessageQueue(Glib::RefPtr<Glib::MainContext> context)
        : m_context(context)
        , m_mutex(std::make_shared<std::recursive_mutex>())
    {
    }

    ContextBoundMessageQueue::~ContextBoundMessageQueue()
    {
      std::unique_lock<std::recursive_mutex> lock(*m_mutex);
      m_jobs.clear();
      lock.unlock();
    }

    void ContextBoundMessageQueue::pushMessage(tMessage &&m)
    {
      std::unique_lock<std::recursive_mutex> lock(*m_mutex);
      m_jobs.remove_if([&](const auto &a) { return a.unique(); });
      m_pendingCalls++;

      auto job = std::make_shared<Job>([this, m = std::move(m)]() {
        m_pendingCalls--;
        m();
      });

      m_jobs.push_back(job);

      lock.unlock();

      m_context->invoke([jobCopy = job, mutex = m_mutex] {
        std::unique_lock<std::recursive_mutex> lock(*mutex);
        if(!jobCopy.unique())
        {
          lock.unlock();
          (*jobCopy)();
        }
        return false;
      });
    }

    bool ContextBoundMessageQueue::isPending() const
    {
      return m_pendingCalls > 0;
    }

    Glib::RefPtr<Glib::MainContext> ContextBoundMessageQueue::getContext()
    {
      return m_context;
    }
  }
}
