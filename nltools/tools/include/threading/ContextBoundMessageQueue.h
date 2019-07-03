#pragma once

#include <glibmm/main.h>
#include <functional>
#include <mutex>
#include <list>

namespace nltools
{
  namespace threading
  {
    class ContextBoundMessageQueue
    {
     public:
      using tMessage = std::function<void(void)>;
      explicit ContextBoundMessageQueue(Glib::RefPtr<Glib::MainContext> context);
      void pushMessage(tMessage&& m);

     protected:
      Glib::RefPtr<Glib::MainContext> m_context;
      int m_signal = 0;

     private:
      static gboolean doQueuedStuff(ContextBoundMessageQueue* pThis);

      std::list<tMessage> m_queue;
      std::recursive_mutex m_mutex;
    };
  }
}
