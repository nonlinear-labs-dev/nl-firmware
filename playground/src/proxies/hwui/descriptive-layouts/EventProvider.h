#pragma once

#include "TemplateEnums.h"
#include <experimental/any>

namespace DescriptiveLayouts
{

  class EventProvider
  {
   public:
    static std::unique_ptr<EventProvider> instantiate(EventProviders e);
    virtual ~EventProvider();

    using Callback = std::function<void(std::experimental::any)>;

    virtual sigc::connection connect(EventSources source, const Callback& cb);
    virtual void fire(EventSinks e);

   protected:
    EventProvider();
  };
}
