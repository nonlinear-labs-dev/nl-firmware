#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/AftertouchCurve.h>
#include <proxies/hwui/panel-unit/boled/setup/MenuEditorEntry.h>
#include <proxies/hwui/panel-unit/boled/setup/AftertouchEditor.h>
#include "use-cases/SettingsUseCases.h"

AftertouchEditor::AftertouchEditor()
{
  Application::get().getSettings()->getSetting<AftertouchCurve>()->onChange(
      mem_fun(this, &AftertouchEditor::onSettingChanged));
}

AftertouchEditor::~AftertouchEditor()
{
}

void AftertouchEditor::incSetting(int inc)
{
  Application::get().getSettings()->getSetting<AftertouchCurve>()->inc(inc, false);
}

bool AftertouchEditor::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down && i == Buttons::BUTTON_DEFAULT)
  {
    SettingsUseCases settingsUseCases(*Application::get().getSettings());
    settingsUseCases.factoryDefaultSetting(Application::get().getSettings()->getSetting<AftertouchCurve>());
    return true;
  }
  return MenuEditor::onButton(i, down, modifiers);
}

const std::vector<Glib::ustring> &AftertouchEditor::getDisplayStrings() const
{
  return Application::get().getSettings()->getSetting<AftertouchCurve>()->enumToDisplayString();
}

int AftertouchEditor::getSelectedIndex() const
{
  return (int) Application::get().getSettings()->getSetting<AftertouchCurve>()->get();
}
