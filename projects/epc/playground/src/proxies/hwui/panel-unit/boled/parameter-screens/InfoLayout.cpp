#include "InfoLayout.h"
#include "controls/MultiLineLabel.h"
#include "controls/ScrollArea.h"
#include "use-cases/SettingsUseCases.h"
#include <proxies/hwui/panel-unit/boled/preset-screens/PresetManagerLayout.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUI.h>
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/FocusAndModeSetting.h>

InfoLayout::InfoLayout()
    : super(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
{
}

void InfoLayout::init()
{
  super::init();

  addModuleCaption();
  addHeadline();
  addInfoLabel();

  m_scrollArea = addControl(new ScrollArea(createScrollableContent(), getScrollableAreaRect()));
}

Rect InfoLayout::getScrollableAreaRect() const
{
  return Rect(0, 14, 256, 50);
}

bool InfoLayout::onRotary(int inc, ButtonModifiers modifiers)
{
  m_scrollArea->scroll(inc * 2);
  return true;
}

void InfoLayout::scrollToMax()
{
  m_scrollArea->scroll(std::numeric_limits<int>::max() / 2);
}

bool InfoLayout::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  const auto focusAndMode = Application::get().getSettings()->getSetting<FocusAndModeSetting>()->getState();
  SettingsUseCases useCases(*Application::get().getSettings());

  if(down)
  {
    switch(i)
    {
      case Buttons::BUTTON_A:
        if(focusAndMode.mode == UIMode::Info)
        {
          if(focusAndMode.focus == UIFocus::Presets)
          {
            useCases.setFocusAndMode(FocusAndMode(UIFocus::Banks, UIMode::Info));
          }
          else if(focusAndMode.focus == UIFocus::Banks)
          {
            useCases.setFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Info));
          }
          return true;
        }
        break;
      case Buttons::BUTTON_PRESET:
        if(focusAndMode.focus == UIFocus::Presets)
        {
          useCases.setFocusAndMode(FocusAndMode { UIMode::Select });
        }
        else
        {
          useCases.setFocusAndMode(FocusAndMode { UIFocus::Presets });
        }
        return true;

      case Buttons::BUTTON_STORE:
        useCases.setFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Store));
        return true;

      case Buttons::BUTTON_EDIT:
        useCases.setFocusAndMode(FocusAndMode { UIMode::Edit });
        return true;

      case Buttons::BUTTON_INFO:
      case Buttons::BUTTON_ENTER:
        useCases.setFocusAndMode(FocusAndMode { UIMode::Select });
        return true;
    }
  }

  return super::onButton(i, down, modifiers);
}
