#pragma once

#include "ControlInstance.h"
#include "proxies/hwui/controls/ControlWithChildren.h"

namespace DescriptiveLayouts
{
  class GenericLayout;

  class GenericControl : public ControlWithChildren
  {
   public:
    GenericControl(const ControlInstance& prototype);
    virtual ~GenericControl();

    void style(const LayoutClasses& layout);
    void connect();

   private:
    void addPrimitives();
    void onEventFired(const std::experimental::any& v, const ControlInstance::EventConnection& c);

   private:
    std::list<sigc::connection> m_connections;

   protected:
    ControlInstance m_prototype;
    LayoutClasses m_lastUsedLayout;
  };
}
