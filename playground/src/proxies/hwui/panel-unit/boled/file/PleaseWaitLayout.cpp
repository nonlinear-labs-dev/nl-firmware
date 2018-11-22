#include <proxies/hwui/controls/Label.h>
#include <Application.h>
#include "PleaseWaitLayout.h"
#include <proxies/hwui/HWUI.h>

PleaseWaitLayout::PleaseWaitLayout()
    : DFBLayout(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
{
  addControl(new Label("Please Wait!", Rect(0, 10, 250, 10)));
  addControl(new Label("This will not take longer than a few Seconds...", Rect(0, 25, 250, 10)));
}

bool PleaseWaitLayout::onButton(int i, bool down, ButtonModifiers modifiers)
{
  return false;
}
