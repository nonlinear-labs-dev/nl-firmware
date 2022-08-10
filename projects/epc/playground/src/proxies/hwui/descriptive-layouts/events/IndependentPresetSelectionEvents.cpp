#include "IndependentPresetSelectionEvents.h"
#include "proxies/hwui/descriptive-layouts/primitives/Text.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "presets/Bank.h"
#include "presets/Preset.h"
#include "event-sources/base/EventSource.h"
#include "GlobalEventSinkBroker.h"
#include "GlobalEventSourceBroker.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/EditBufferUseCases.h"
#include <proxies/hwui/HWUI.h>

namespace DescriptiveLayouts
{

  IndependentPresetSelectionEvents::IndependentPresetSelectionEvents()
      : m_cursor([](const Preset *preset) { return preset->getType() == SoundType::Single; })
  {
    createEventSources<Text::DisplayString>(
        EventSources::PreviousNumber, EventSources::PreviousName, EventSources::CurrentNumber,
        EventSources::CurrentName, EventSources::NextNumber, EventSources::NextName, EventSources::PresetListBankName);
    createEventSources<bool>(EventSources::CanLeft, EventSources::CanRight);

    bruteForce();
  }

  IndependentPresetSelectionEvents::~IndependentPresetSelectionEvents() = default;

  sigc::connection IndependentPresetSelectionEvents::connect(EventSources source, const Callback &cb)
  {
    return eventSources.at(source)->connect(cb);
  }

  void IndependentPresetSelectionEvents::fire(EventSinks e)
  {
    switch(e)
    {
      case EventSinks::Left:
        if(m_cursor.canPreviousBank())
          m_cursor.previousBank();
        break;

      case EventSinks::Right:
        if(m_cursor.canNextBank())
          m_cursor.nextBank();
        break;

      case EventSinks::Up:
        if(m_cursor.canPreviousPreset())
          m_cursor.previousPreset();
        break;

      case EventSinks::Down:
        if(m_cursor.canNextPreset())
          m_cursor.nextPreset();
        break;

      case EventSinks::Commit:
        if(auto preset = m_cursor.getPreset())
        {
          auto eb = Application::get().getPresetManager()->getEditBuffer();
          EditBufferUseCases useCase(*eb);
          auto currentVG = Application::get().getVGManager()->getCurrentVoiceGroup();
          useCase.loadToPart(preset, VoiceGroup::I, currentVG);
          SettingsUseCases useCases(*Application::get().getSettings());
          useCases.setFocusAndMode(FocusAndMode(UIFocus::Sound, UIMode::Select, UIDetail::Init));
        }
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

      auto currentCursor = m_cursor;
      auto previousCursor = currentCursor;
      previousCursor.previousPreset();
      auto nextCursor = currentCursor;
      nextCursor.nextPreset();

      setString(EventSources::PreviousNumber, previousCursor.getPresetNumberString());
      setString(EventSources::PreviousName, previousCursor.getPresetName());
      setString(EventSources::CurrentNumber, currentCursor.getPresetNumberString());
      setString(EventSources::CurrentName, currentCursor.getPresetName());
      setString(EventSources::NextNumber, nextCursor.getPresetNumberString());
      setString(EventSources::NextName, nextCursor.getPresetName());

      setString(EventSources::PresetListBankName, currentCursor.getBankName());

      static_cast<EventSource<bool> *>(eventSources.at(EventSources::CanLeft).get())
          ->setValue(currentCursor.canPreviousBank());
      static_cast<EventSource<bool> *>(eventSources.at(EventSources::CanRight).get())
          ->setValue(currentCursor.canNextBank());

      if(auto b = currentCursor.getBank())
        connections.push_back(b->onBankChanged(sigc::mem_fun(this, &IndependentPresetSelectionEvents::bruteForce)));

      if(auto p = previousCursor.getPreset())
        connections.push_back(p->onChanged(sigc::mem_fun(this, &IndependentPresetSelectionEvents::bruteForce)));

      if(auto p = currentCursor.getPreset())
        connections.push_back(p->onChanged(sigc::mem_fun(this, &IndependentPresetSelectionEvents::bruteForce)));

      if(auto p = nextCursor.getPreset())
        connections.push_back(p->onChanged(sigc::mem_fun(this, &IndependentPresetSelectionEvents::bruteForce)));
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
