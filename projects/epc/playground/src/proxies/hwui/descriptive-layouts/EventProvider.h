#pragma once

#include "TemplateEnums.h"
#include <experimental/any>
#include <sigc++/connection.h>

namespace DescriptiveLayouts
{
  class GlobalEventSourceBroker;
  class GlobalEventSinkBroker;

  GlobalEventSourceBroker &getGlobalEventSourceBroker();
  GlobalEventSinkBroker &getGlobalEventSinkBroker();

  class EventProvider : public sigc::trackable
  {
   public:
    static std::unique_ptr<EventProvider> instantiate(EventProviders e);
    virtual ~EventProvider();

    using Callback = std::function<void(std::experimental::any)>;

    virtual sigc::connection connect(EventSources source, const Callback &cb);
    virtual void fire(EventSinks e);

   protected:
    EventProvider();
  };
}
