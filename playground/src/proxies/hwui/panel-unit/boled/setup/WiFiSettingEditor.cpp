#include <device-settings/WifiSetting.h>
#include <Application.h>
#include "WiFiSettingEditor.h"
#include <device-settings/Settings.h>

WiFiSettingEditor::WiFiSettingEditor()
{
  Application::get().getSettings()->getSetting<WifiSetting>()->onChange(
      mem_fun(this, &WiFiSettingEditor::onSettingChanged));
}

WiFiSettingEditor::~WiFiSettingEditor() = default;

void WiFiSettingEditor::incSetting(int inc)
{
  auto setting = Application::get().getSettings()->getSetting<WifiSetting>();
  setting->inc(1, true);
}

const std::vector<Glib::ustring> &WiFiSettingEditor::getDisplayStrings() const
{
  return Application::get().getSettings()->getSetting<WifiSetting>()->enumToDisplayString();
}

int WiFiSettingEditor::getSelectedIndex() const
{
  auto enabled = Application::get().getSettings()->getSetting<WifiSetting>()->get();
  return enabled ? 0 : 1;
}
