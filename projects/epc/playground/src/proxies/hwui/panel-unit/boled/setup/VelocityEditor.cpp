#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/VelocityCurve.h>
#include <proxies/hwui/panel-unit/boled/setup/MenuEditorEntry.h>
#include <proxies/hwui/panel-unit/boled/setup/VelocityEditor.h>
#include "use-cases/SettingsUseCases.h"

VelocityEditor::VelocityEditor()
{
  Application::get().getSettings()->getSetting<VelocityCurve>()->onChange(
      mem_fun(this, &VelocityEditor::onSettingChanged));
}

VelocityEditor::~VelocityEditor()
{
}

void VelocityEditor::incSetting(int inc)
{
  Application::get().getSettings()->getSetting<VelocityCurve>()->inc(inc, false);
}

const std::vector<Glib::ustring> &VelocityEditor::getDisplayStrings() const
{
  return Application::get().getSettings()->getSetting<VelocityCurve>()->enumToDisplayString();
}

bool VelocityEditor::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down && i == Buttons::BUTTON_DEFAULT)
  {
    SettingsUseCases settingsUseCases(*Application::get().getSettings());
    settingsUseCases.factoryDefaultSetting(Application::get().getSettings()->getSetting<VelocityCurve>());
    return true;
  }
  return MenuEditor::onButton(i, down, modifiers);
}

int VelocityEditor::getSelectedIndex() const
{
  return (int) Application::get().getSettings()->getSetting<VelocityCurve>()->get();
}
