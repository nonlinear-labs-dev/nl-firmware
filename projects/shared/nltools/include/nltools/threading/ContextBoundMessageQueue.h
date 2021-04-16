#pragma once

#include <nltools/GlibFwd.h>
#include <functional>
#include <sigc++/trackable.h>
#include <glibmm/refptr.h>
#include <atomic>
#include <memory>
#include <list>
#include <mutex>

namespace nltools
{
  namespace threading
  {
    class ContextBoundMessageQueue : public sigc::trackable
    {
     public:
      using tMessage = std::function<void(void)>;
      explicit ContextBoundMessageQueue(Glib::RefPtr<Glib::MainContext> context);
      ~ContextBoundMessageQueue();

      void pushMessage(tMessage&& m);
      bool isPending() const;

     private:
      Glib::RefPtr<Glib::MainContext> m_context;
      std::atomic<uint32_t> m_pendingCalls {};

      using Job = std::function<void()>;
      using tJob = std::shared_ptr<Job>;
      std::list<tJob> m_jobs;
      std::shared_ptr<std::mutex> m_mutex;
    };
  }
}
