#pragma once

#include <proxies/hwui/descriptive-layouts/EventProvider.h>
#include <presets/PresetManagerCursor.h>

namespace DescriptiveLayouts
{
  class EventSourceBase;

  class PresetSelectionEvents : public EventProvider
  {
   public:
    PresetSelectionEvents(const PresetManagerCursor::Matcher& filter);
    ~PresetSelectionEvents() override;

    sigc::connection connect(EventSources source, const Callback &cb) override;
    void fire(EventSinks e) override;

   private:
    void bruteForce();
    void sanitize();
    void setString(EventSources e, const std::string &str);

    template <typename T, typename... P> void createEventSources(P... e);

   private:
    PresetManagerCursor m_cursor;
    std::list<sigc::connection> connections;
    UpdateDocumentContributor::tUpdateID lastSeenPresetManagerRevision = -1;

    std::map<EventSources, std::unique_ptr<EventSourceBase>> eventSources;
  };
}
