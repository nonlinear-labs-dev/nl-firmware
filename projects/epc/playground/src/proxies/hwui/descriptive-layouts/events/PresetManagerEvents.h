#pragma once

#include <proxies/hwui/descriptive-layouts/EventProvider.h>
#include <nltools/threading/Throttler.h>

namespace DescriptiveLayouts
{
  class EventSourceBase;

  class PresetManagerEvents : public EventProvider
  {
   public:
    PresetManagerEvents();
    ~PresetManagerEvents() override;

    sigc::connection connect(EventSources source, const Callback &cb) override;
    void fire(EventSinks e) override;

   private:
    void bruteForce();
    void updateDirectLoad();

    void setString(EventSources e, const std::string &str);
    void setBool(EventSources e, bool b);

    template <typename T, typename... P> void createEventSources(P... e);

   private:
    std::list<sigc::connection> connections;
    int lastSeenPresetManagerRevision = -1;

    std::map<EventSources, std::unique_ptr<EventSourceBase>> eventSources;
    Throttler m_throttler;
  };
}
