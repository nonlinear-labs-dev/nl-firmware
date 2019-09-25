#include "IndependentPresetSelectionEvents.h"
#include "proxies/hwui/descriptive-layouts/primitives/Text.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/Bank.h"
#include "presets/Preset.h"
#include "event-sources/base/EventSource.h"
#include "GlobalEventSinkBroker.h"
#include "GlobalEventSourceBroker.h"

namespace DescriptiveLayouts
{
  IndependentPresetSelectionEvents::IndependentPresetSelectionEvents()
  {
    createEventSources<Text::DisplayString>(
        EventSources::PreviousNumber, EventSources::PreviousName, EventSources::CurrentNumber,
        EventSources::CurrentName, EventSources::NextNumber, EventSources::NextName, EventSources::PresetListBankName);
    createEventSources<bool>(EventSources::CanLeft, EventSources::CanRight);

    auto pm = Application::get().getPresetManager();
    if(auto b = pm->getSelectedBank())
    {
      selectedBankNumber = pm->getBankPosition(b->getUuid());

      if(auto p = b->getSelectedPreset())
        selectedPresetNumber = b->getPresetPosition(p);
    }

    bruteForce();
  }

  IndependentPresetSelectionEvents::~IndependentPresetSelectionEvents() = default;

  connection IndependentPresetSelectionEvents::connect(EventSources source, const Callback &cb)
  {
    return eventSources.at(source)->connect(cb);
  }

  void IndependentPresetSelectionEvents::fire(EventSinks e)
  {
    switch(e)
    {
      case EventSinks::Left:
        if(selectedBankNumber)
          selectedBankNumber--;
        break;

      case EventSinks::Right:
        selectedBankNumber++;
        break;

      case EventSinks::Up:
        selectedPresetNumber++;
        break;

      case EventSinks::Down:
        if(selectedPresetNumber)
          selectedPresetNumber--;
        break;

      default:
        getGlobalEventSinkBroker().fire(e);
    }

    lastSeenPresetManagerRevision = -1;
    bruteForce();
  }

  void IndependentPresetSelectionEvents::bruteForce()
  {
    auto pm = Application::get().getPresetManager();
    auto updateId = pm->getUpdateIDOfLastChange();

    if(std::exchange(lastSeenPresetManagerRevision, updateId) != updateId)
    {
      connections.clear();

      sanitize();

      auto bank = pm->getBankAt(selectedBankNumber);
      auto numPresets = bank ? bank->getNumPresets() : 0;
      auto previousPreset = bank && selectedPresetNumber ? bank->getPresetAt(selectedPresetNumber - 1) : nullptr;
      auto currentPreset = bank && selectedPresetNumber >= 0 && selectedPresetNumber < numPresets
          ? bank->getPresetAt(selectedPresetNumber)
          : nullptr;
      auto nextPreset
          = bank && selectedPresetNumber < numPresets - 1 ? bank->getPresetAt(selectedPresetNumber + 1) : nullptr;

      setString(EventSources::PreviousNumber,
                previousPreset ? std::to_string(bank->getPresetPosition(previousPreset->getUuid())) : "");
      setString(EventSources::PreviousName, previousPreset ? previousPreset->getName() : "");
      setString(EventSources::CurrentNumber,
                currentPreset ? std::to_string(bank->getPresetPosition(currentPreset->getUuid())) : "");
      setString(EventSources::CurrentName, currentPreset ? currentPreset->getName() : "");

      setString(EventSources::NextNumber,
                nextPreset ? std::to_string(bank->getPresetPosition(nextPreset->getUuid())) : "");
      setString(EventSources::NextName, nextPreset ? nextPreset->getName() : "");

      setString(EventSources::PresetListBankName, bank ? bank->getName(true) : "");

      static_cast<EventSource<bool> *>(eventSources.at(EventSources::CanLeft).get())->setValue(selectedBankNumber > 0);
      static_cast<EventSource<bool> *>(eventSources.at(EventSources::CanRight).get())
          ->setValue(selectedBankNumber < pm->getNumBanks() - 1);

      if(bank)
        connections.push_back(bank->onBankChanged(sigc::mem_fun(this, &IndependentPresetSelectionEvents::bruteForce)));

      if(previousPreset)
        connections.push_back(
            previousPreset->onChanged(sigc::mem_fun(this, &IndependentPresetSelectionEvents::bruteForce)));

      if(currentPreset)
        connections.push_back(
            currentPreset->onChanged(sigc::mem_fun(this, &IndependentPresetSelectionEvents::bruteForce)));

      if(nextPreset)
        connections.push_back(
            nextPreset->onChanged(sigc::mem_fun(this, &IndependentPresetSelectionEvents::bruteForce)));
    }
  }

  void IndependentPresetSelectionEvents::sanitize()
  {
    auto pm = Application::get().getPresetManager();

    if(auto numBanks = pm->getNumBanks())
    {
      selectedBankNumber = CLAMP(selectedBankNumber, 0, numBanks - 1);
      auto b = pm->getBankAt(selectedBankNumber);

      if(auto numPresets = b->getNumPresets())
        selectedPresetNumber = CLAMP(selectedPresetNumber, 0, numPresets - 1);
      else
        selectedPresetNumber = invalid;
    }
    else
    {
      selectedBankNumber = invalid;
      selectedPresetNumber = invalid;
    }
  }

  void IndependentPresetSelectionEvents::setString(EventSources e, const std::string &str)
  {
    static_cast<EventSource<Text::DisplayString> *>(eventSources.at(e).get())->setValue({ str, 0 });
  }

  template <typename T, typename... P> void IndependentPresetSelectionEvents::createEventSources(P... e)
  {
    using D = EventSource<T>;
    std::initializer_list<bool>({ (eventSources[e] = std::make_unique<D>(), false)... });
  }
}
