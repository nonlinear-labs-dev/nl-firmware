#include <nltools/threading/ContextBoundMessageQueue.h>
#include <nltools/logging/Log.h>
#include <glibmm.h>
#include <thread>

using namespace std::chrono_literals;

namespace nltools
{
  namespace threading
  {
    ContextBoundMessageQueue::ContextBoundMessageQueue(Glib::RefPtr<Glib::MainContext> context)
        : m_context(std::move(context))
        , m_mutex(std::make_shared<std::mutex>())
    {
    }

    ContextBoundMessageQueue::~ContextBoundMessageQueue()
    {
      std::unique_lock<std::mutex> lock(*m_mutex);
      m_jobs.clear();
      m_pendingCalls = 0;
    }

    void ContextBoundMessageQueue::pushMessage(tMessage &&m)
    {
      m_jobs.remove_if([&](const auto &a) { return a.unique(); });

      m_pendingCalls++;

      auto job = std::make_shared<Job>([this, m = std::move(m)]() {
        m_pendingCalls--;
        m();
      });

      m_jobs.push_back(job);

      m_context->invoke([jobCopy = job, mutex = m_mutex] {
        std::unique_lock<std::mutex> lock(*mutex);
        if(!jobCopy.unique())
          (*jobCopy)();
        return false;
      });
    }

    bool ContextBoundMessageQueue::isPending() const
    {
      return m_pendingCalls > 0;
    }
  }
}
