#include <device-settings/WifiSetting.h>
#include <Application.h>
#include "WiFiSettingEditor.h"

WiFiSettingEditor::WiFiSettingEditor() {
    Application::get().getSettings()->getSetting<WifiSetting>()->onChange(
            mem_fun(this, &WiFiSettingEditor::onSettingChanged));
}

WiFiSettingEditor::~WiFiSettingEditor() = default;

void WiFiSettingEditor::incSetting(int inc) {
    Application::get().getSettings()->getSetting<WifiSetting>()->inc(inc, false);
}

const vector<ustring> &WiFiSettingEditor::getDisplayStrings() const {
    return Application::get().getSettings()->getSetting<WifiSetting>()->enumToDisplayString();
}

int WiFiSettingEditor::getSelectedIndex() const {
    auto enabled = Application::get().getSettings()->getSetting<WifiSetting>()->get();
    return enabled ? 0 : 1;
}
