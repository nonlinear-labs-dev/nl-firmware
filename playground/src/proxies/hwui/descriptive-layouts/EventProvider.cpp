#include "EventProvider.h"
#include "events/GlobalEventSinkBroker.h"
#include "events/GlobalEventSourceBroker.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/Bank.h"
#include "presets/Preset.h"

namespace DescriptiveLayouts
{
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

  struct PresetSelectionForVoiceGroup : EventProvider
  {
    PresetSelectionForVoiceGroup()
    {
      auto pm = Application::get().getPresetManager();
      if(auto b = pm->getSelectedBank())
      {
        selectedBankNumber = pm->getBankPosition(b->getUuid());

        if(auto p = b->getSelectedPreset())
        {
          selectedPresetNumber = b->getPresetPosition(p);
        }
      }

      bankSelectionChangedConnection
          = pm->onBankSelection(sigc::mem_fun(this, &PresetSelectionForVoiceGroup::onBankSelection));
    }

    ~PresetSelectionForVoiceGroup() override = default;

    connection connect(EventSources source, const Callback &cb) override
    {
      return eventSources[source].connect(cb);
    }

    void onBankSelection(const Uuid &uuid)
    {
      presetSelectionChangedConnection.disconnect();
      auto pm = Application::get().getPresetManager();
      if(auto b = pm->getSelectedBank())
        presetSelectionChangedConnection
            = b->onBankChanged(sigc::mem_fun(this, &PresetSelectionForVoiceGroup::bruteForce));
    }

    void fire(EventSinks e) override
    {
      getGlobalEventSinkBroker().fire(e);
    }

    void bruteForce()
    {
      eventSources[EventSources::PreviousNumber].send("1");
      eventSources[EventSources::PreviousName].send("1");
      eventSources[EventSources::CurrentNumber].send("1");
      eventSources[EventSources::CurrentName].send("1");
      eventSources[EventSources::NextNumber].send("1");
      eventSources[EventSources::NextName].send("1");
    }

   private:
    std::map<EventSources, Signal<void, std::string>> eventSources;

    constexpr static size_t invalid = -1;
    size_t selectedBankNumber = invalid;
    size_t selectedPresetNumber = invalid;
    sigc::connection bankSelectionChangedConnection;
    sigc::connection presetSelectionChangedConnection;
  };

  EventProvider::EventProvider() = default;
  EventProvider::~EventProvider() = default;

  std::unique_ptr<EventProvider> EventProvider::instantiate(EventProviders e)
  {
    switch(e)
    {
      case EventProviders::Global:
        return std::make_unique<GlobalEventProvider>();

      case EventProviders::PresetSelectionForVoiceGroup:
        return std::make_unique<PresetSelectionForVoiceGroup>();
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
