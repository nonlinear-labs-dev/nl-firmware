#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/SignalFlowIndicationSetting.h>
#include <proxies/hwui/HWUI.h>
#include "SetupLabel.h"
#include "SignalFlowIndicatorEditor.h"
#include <proxies/hwui/Font.h>
#include <device-settings/DebugLevel.h>
#include <proxies/hwui/UsageMode.h>

SignalFlowIndicatorEditor::SignalFlowIndicatorEditor()
    : super()
{
  Application::get().getSettings()->getSetting<SignalFlowIndicationSetting>()->onChange(
      mem_fun(this, &SignalFlowIndicatorEditor::onSettingChanged));
}

void SignalFlowIndicatorEditor::incSetting(int inc)
{
  Application::get().getSettings()->getSetting<SignalFlowIndicationSetting>()->inc(inc, false);
}

const vector<ustring> &SignalFlowIndicatorEditor::getDisplayStrings() const
{
  return Application::get().getSettings()->getSetting<SignalFlowIndicationSetting>()->enumToDisplayString();
}

int SignalFlowIndicatorEditor::getSelectedIndex() const
{
  auto enabled = Application::get().getSettings()->getSetting<SignalFlowIndicationSetting>()->get();
  return enabled ? 0 : 1;
}
