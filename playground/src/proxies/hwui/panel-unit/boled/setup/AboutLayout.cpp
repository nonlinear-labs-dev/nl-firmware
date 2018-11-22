#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ScrollArea.h>
#include <proxies/hwui/panel-unit/boled/setup/AboutLayout.h>
#include <proxies/hwui/panel-unit/boled/setup/AboutText.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>

AboutLayout::AboutLayout()
    : super(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
{
  m_scrollArea = addControl(new ScrollArea(new AboutText(), Rect(0, 0, 256, 64)));
}

AboutLayout::~AboutLayout()
{
}

bool AboutLayout::onRotary(int inc, ButtonModifiers modifiers)
{
  m_scrollArea->scroll(inc);
  return true;
}

bool AboutLayout::onButton(int i, bool down, ButtonModifiers modifiers)
{
  if(!super::onButton(i, down, modifiers))
  {
    if(down)
    {
      Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().resetOverlay();

      if(i == BUTTON_PRESET)
      {
        Application::get().getHWUI()->undoableSetFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Select));
      }
      else if(i == BUTTON_STORE)
      {
        Application::get().getHWUI()->undoableSetFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Store));
      }
    }
  }

  return true;
}
