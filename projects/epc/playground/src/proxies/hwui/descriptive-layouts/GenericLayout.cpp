#include <utility>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulationCarousel.h>
#include <proxies/hwui/panel-unit/ButtonReceiver.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterCarousel.h>
#include "GenericLayout.h"
#include "GenericControl.h"

#include "Application.h"
#include "proxies/hwui/controls/ButtonMenu.h"
#include "proxies/hwui/HWUI.h"
#include "proxies/hwui/controls/ControlAdapters.h"
#include "EventProvider.h"
#include "ConditionRegistry.h"
#include "use-cases/SettingsUseCases.h"

namespace DescriptiveLayouts
{
  GenericLayout::GenericLayout(LayoutClass prototype)
      : Layout(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
      , m_prototype(std::move(prototype))
  {
  }

  GenericLayout::~GenericLayout()
  {
    m_onConditionsChanged.disconnect();
  }

  void GenericLayout::init()
  {
    m_eventProvider = EventProvider::instantiate(m_prototype.eventProvider);
    super::init();
    createControls();
    connectConditions();
  }

  void GenericLayout::createControls()
  {
    for(auto &c : m_prototype.controls)
    {
      auto control = c.instantiate(m_eventProvider.get());
      if(auto g = dynamic_cast<GenericControl *>(control))
      {
        g->style(m_prototype.id);
        g->connect();
      }
      addControl(control);
    }
  }

  void GenericLayout::connectConditions()
  {
    m_onConditionsChanged = ConditionRegistry::get().onChange([this]() {
      if(!m_prototype.meetsConditions())
        Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().bruteForce();
    });
  }

  template <class CB> bool traverse(Control *c, CB cb)
  {
    if(auto cc = dynamic_cast<ControlOwner *>(c))
    {
      for(auto &control : cc->getControls())
      {
        if(traverse(control.get(), cb))
          return true;
      }
    }

    if(auto receiver = dynamic_cast<ButtonReceiver *>(c))
    {
      return cb(receiver);
    }
    else
    {
      return false;
    }
  }

  bool GenericLayout::onButton(Buttons i, bool down, ::ButtonModifiers modifiers)
  {
    if(!down)
      removeButtonRepeat();

    for(auto &c : getControls())
    {
      if(traverse(c.get(), [=](ButtonReceiver *r) -> bool { return r->onButton(i, down, modifiers); }))
        return true;
    }

    if(down)
    {
      for(const EventSinkMapping &m : m_prototype.sinkMappings)
      {
        if(m.button == i)
        {
          if(!handleEventSink(m.sink))
          {
            if(m.repeat)
              installButtonRepeat([=]() { m_eventProvider->fire(m.sink); });
            else
              m_eventProvider->fire(m.sink);
          }
          return true;
        }
      }
    }
    return handleDefaults(i, down, modifiers);
  }

  bool GenericLayout::handleDefaults(Buttons buttons, bool down, ::ButtonModifiers modifiers)
  {
    if(down)
    {
      switch(buttons)
      {
        case Buttons::BUTTON_PRESET:
          togglePresetMode();
          return true;
        case Buttons::BUTTON_SOUND:
          toggleSoundMode();
          return true;
        case Buttons::BUTTON_SETUP:
          toggleSetupMode();
          return true;
        case Buttons::BUTTON_STORE:
          toggleStoreMode();
          return true;
        case Buttons::BUTTON_INFO:
          toggleInfo();
          return true;
        case Buttons::BUTTON_EDIT:
          toggleEdit();
          return true;
      }
    }
    return false;
  }

  const LayoutClass &GenericLayout::getPrototype() const
  {
    return m_prototype;
  }

  bool GenericLayout::handleEventSink(EventSinks s)
  {
    auto shiftPressed = Application::get().getHWUI()->getButtonModifiers()[ButtonModifier::SHIFT];

    switch(s)
    {
      case EventSinks::IncButtonMenu:
        if(ControlList<ButtonMenu, ModulationCarousel, ParameterCarousel>::findFirstAndCall(
               this, [&](auto m) { controladapters::incDec(m, shiftPressed ? -1 : 1); }))
          return true;
        break;

      case EventSinks::DecButtonMenu:
        if(ControlList<ButtonMenu, ModulationCarousel, ParameterCarousel>::findFirstAndCall(
               this, [&](auto m) { controladapters::incDec(m, shiftPressed ? 1 : -1); }))
          return true;
        break;

      case EventSinks::FireButtonMenu:
        if(auto m = findControlOfType<ButtonMenu>())
        {
          m->doAction();
          return true;
        }
        break;

      case EventSinks::IncParam:
        if(auto car = findControlOfType<ModulationCarousel>())
        {
          car->onRotary(1, Application::get().getHWUI()->getButtonModifiers());
          return true;
        }
        break;

      case EventSinks::DecParam:
        if(auto car = findControlOfType<ModulationCarousel>())
        {
          car->onRotary(-1, Application::get().getHWUI()->getButtonModifiers());
          return true;
        }
        break;

      default:
        break;
    }
    return false;
  }

  bool GenericLayout::onRotary(int inc, ::ButtonModifiers modifiers)
  {
    while(inc > 0)
    {
      onButton(Buttons::ROTARY_PLUS, true, modifiers);
      inc--;
    }

    while(inc < 0)
    {
      onButton(Buttons::ROTARY_MINUS, true, modifiers);
      inc++;
    }
    return true;
  }

  void GenericLayout::togglePresetMode()
  {
    SettingsUseCases useCases(*Application::get().getSettings());
    auto *hwui = Application::get().getHWUI();
    auto current = hwui->getFocusAndModeState();
    if(current.focus == UIFocus::Presets)
      useCases.setFocusAndMode(hwui->getOldFocusAndModeState());
    else
      useCases.setFocusAndMode({ UIFocus::Presets, UIMode::Select, UIDetail::Init });
  }

  void GenericLayout::toggleSoundMode()
  {
    SettingsUseCases useCases(*Application::get().getSettings());
    auto *hwui = Application::get().getHWUI();
    auto current = hwui->getFocusAndModeState();
    if(current.focus == UIFocus::Sound)
      if(current.detail != UIDetail::Init && current.detail != UIDetail::ButtonA)
      {
        useCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
      }
      else
      {
        useCases.setFocusAndMode(hwui->getOldFocusAndModeState());
      }
    else
      useCases.setFocusAndMode({ UIFocus::Sound, UIMode::Select, UIDetail::Init });
  }

  void GenericLayout::toggleSetupMode()
  {
    SettingsUseCases useCases(*Application::get().getSettings());
    auto *hwui = Application::get().getHWUI();
    auto current = hwui->getFocusAndModeState();
    if(current.focus == UIFocus::Setup)
      useCases.setFocusAndMode(hwui->getOldFocusAndModeState());
    else
      useCases.setFocusAndMode({ UIFocus::Setup, UIMode::Select, UIDetail::Init });
  }

  void GenericLayout::toggleStoreMode()
  {
    SettingsUseCases useCases(*Application::get().getSettings());

    auto *hwui = Application::get().getHWUI();
    auto current = hwui->getFocusAndModeState();
    if(current.focus == UIFocus::Presets && current.mode == UIMode::Store)
      useCases.setFocusAndMode({ UIFocus::Presets, UIMode::Select, UIDetail::Init });
    else
      useCases.setFocusAndMode({ UIFocus::Presets, UIMode::Store, UIDetail::Init });
  }

  void GenericLayout::toggleInfo()
  {
    SettingsUseCases useCases(*Application::get().getSettings());

    auto *hwui = Application::get().getHWUI();
    auto current = hwui->getFocusAndModeState();
    if(current.mode == UIMode::Info)
      useCases.setFocusAndMode({ UIFocus::Unchanged, UIMode::Select, UIDetail::Init });
    else
      useCases.setFocusAndMode({ UIFocus::Unchanged, UIMode::Info, UIDetail::Init });
  }

  void GenericLayout::toggleEdit()
  {
    SettingsUseCases useCases(*Application::get().getSettings());
    auto *hwui = Application::get().getHWUI();
    auto current = hwui->getFocusAndModeState();

    if(current.mode == UIMode::Edit)
      useCases.setFocusAndMode({ UIFocus::Unchanged, UIMode::Select, UIDetail::Init });
    else
      useCases.setFocusAndMode({ UIFocus::Unchanged, UIMode::Edit, UIDetail::Init });
  }

  bool GenericLayout::redraw(FrameBuffer &fb)
  {
    getOLEDProxy().clear();
    return ControlOwner::redraw(fb);
  }
}
