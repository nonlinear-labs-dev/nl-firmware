#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/ScreenSaverTimeoutSetting.h>
#include "ScreenSaverTimeControls.h"

//View
ScreenSaverTimeView::ScreenSaverTimeView()
    : SetupLabel({0,0,0,0})
{
  Application::get().getSettings()->getSetting<ScreenSaverTimeoutSetting>()->onChange(
      mem_fun(this, &ScreenSaverTimeView::onSettingChanged));
}

void ScreenSaverTimeView::onSettingChanged(const Setting* s)
{
  setText({ s->getDisplayString(), 0 });
}

//Editor
ScreenSaverTimeEditor::ScreenSaverTimeEditor()
    : super(Rect(0, 0, 0, 0))
{

  Application::get().getSettings()->getSetting<ScreenSaverTimeoutSetting>()->onChange(
      mem_fun(this, &ScreenSaverTimeEditor::onSettingChanged));
}

ScreenSaverTimeEditor::~ScreenSaverTimeEditor()
{
}

void ScreenSaverTimeEditor::onSettingChanged(const Setting* s)
{
  if(auto p = dynamic_cast<const ScreenSaverTimeoutSetting*>(s))
    setText(p->getDisplayString());
}

bool ScreenSaverTimeEditor::onRotary(int inc, ButtonModifiers modifiers)
{
  Application::get().getSettings()->getSetting<ScreenSaverTimeoutSetting>()->incDec(inc);
  return true;
}
