#include <Application.h>
#include <device-settings/BenderCurve.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/panel-unit/boled/setup/BenderCurveEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/MenuEditorEntry.h>
#include "use-cases/SettingsUseCases.h"

BenderCurveEditor::BenderCurveEditor()
{
  Application::get().getSettings()->getSetting<BenderCurve>()->onChange(
      mem_fun(this, &BenderCurveEditor::onSettingChanged));
}

BenderCurveEditor::~BenderCurveEditor()
{
}

void BenderCurveEditor::incSetting(int inc)
{
  Application::get().getSettings()->getSetting<BenderCurve>()->inc(inc, false);
}

const std::vector<Glib::ustring> &BenderCurveEditor::getDisplayStrings() const
{
  return Application::get().getSettings()->getSetting<BenderCurve>()->enumToDisplayString();
}

bool BenderCurveEditor::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down && i == Buttons::BUTTON_DEFAULT)
  {
    SettingsUseCases settingsUseCases(*Application::get().getSettings());
    settingsUseCases.factoryDefaultSetting(Application::get().getSettings()->getSetting<BenderCurve>());
    return true;
  }
  return MenuEditor::onButton(i, down, modifiers);
}

int BenderCurveEditor::getSelectedIndex() const
{
  return (int) Application::get().getSettings()->getSetting<BenderCurve>()->get();
}
