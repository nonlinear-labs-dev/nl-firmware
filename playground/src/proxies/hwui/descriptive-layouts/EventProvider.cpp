#include "EventProvider.h"
#include "events/GlobalEventSinkBroker.h"
#include "events/GlobalEventSourceBroker.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/Bank.h"
#include "presets/Preset.h"
#include "primitives/Text.h"
#include "events/event-sources/base/EventSource.h"
#include "events/IndependentPresetSelectionEvents.h"
#include "events/PresetManagerEvents.h"

namespace DescriptiveLayouts
{
  EventProvider::EventProvider() = default;
  EventProvider::~EventProvider() = default;

  GlobalEventSourceBroker &getGlobalEventSourceBroker()
  {
    static GlobalEventSourceBroker globalEventSourceBroker;
    return globalEventSourceBroker;
  }

  GlobalEventSinkBroker &getGlobalEventSinkBroker()
  {
    static GlobalEventSinkBroker globalEventSinkBroker;
    return globalEventSinkBroker;
  }

  struct GlobalEventProvider : EventProvider
  {
    connection connect(EventSources source, const Callback &cb) override
    {
      return getGlobalEventSourceBroker().connect(source, cb);
    }

    void fire(EventSinks e) override
    {
      getGlobalEventSinkBroker().fire(e);
    }
  };

  std::unique_ptr<EventProvider> EventProvider::instantiate(EventProviders e)
  {
    switch(e)
    {
      case EventProviders::Global:
        return std::make_unique<GlobalEventProvider>();

      case EventProviders::IndependentPresetSelectionEvents:
        return std::make_unique<IndependentPresetSelectionEvents>();

      case EventProviders::PresetManagerEvents:
        return std::make_unique<PresetManagerEvents>();
    }
    return nullptr;
  }

  connection EventProvider::connect(EventSources source, const Callback &cb)
  {
    return {};
  }

  void EventProvider::fire(EventSinks e)
  {
  }
}
