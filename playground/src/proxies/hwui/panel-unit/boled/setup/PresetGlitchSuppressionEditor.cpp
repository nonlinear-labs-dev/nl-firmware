#include <Application.h>
#include <device-settings/PresetGlitchSuppression.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/panel-unit/boled/setup/PresetGlitchSuppressionEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/MenuEditorEntry.h>

PresetGlitchSuppressionEditor::PresetGlitchSuppressionEditor()
{
  Application::get().getSettings()->getSetting<PresetGlitchSuppression>()->onChange(
      mem_fun(this, &PresetGlitchSuppressionEditor::onSettingChanged));
}

PresetGlitchSuppressionEditor::~PresetGlitchSuppressionEditor()
{
}

void PresetGlitchSuppressionEditor::incSetting(int inc)
{
  Application::get().getSettings()->getSetting<PresetGlitchSuppression>()->inc(inc, false);
}

const vector<ustring> &PresetGlitchSuppressionEditor::getDisplayStrings() const
{
  return Application::get().getSettings()->getSetting<PresetGlitchSuppression>()->enumToDisplayString();
}

int PresetGlitchSuppressionEditor::getSelectedIndex() const
{
  auto enabled = Application::get().getSettings()->getSetting<PresetGlitchSuppression>()->get();
  return enabled ? 0 : 1;
}
