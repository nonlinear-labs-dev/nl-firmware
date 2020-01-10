#pragma once

#include <nltools/GlibFwd.h>
#include <functional>
#include <sigc++/trackable.h>
#include <glibmm/refptr.h>

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

     private:
      Glib::RefPtr<Glib::MainContext> m_context;
    };
  }
}
