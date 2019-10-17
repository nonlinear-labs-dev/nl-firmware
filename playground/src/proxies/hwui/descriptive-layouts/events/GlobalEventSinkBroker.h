#pragma once

#include <playground.h>
#include <proxies/hwui/descriptive-layouts/TemplateEnums.h>

namespace DescriptiveLayouts
{

  class GlobalEventSinkBroker
  {
   public:
    GlobalEventSinkBroker();
    void fire(EventSinks s);

   private:
    using tAction = std::function<void()>;
    void registerEvent(EventSinks sink, tAction action);
    std::map<EventSinks, tAction> m_map;
  };
}
