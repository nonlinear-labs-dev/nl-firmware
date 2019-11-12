#pragma once
#include "MenuEditor.h"
#include "device-settings/Settings.h"
#include "device-settings/Setting.h"
#include <Application.h>

template <typename tSetting> class EnumSettingEditor : public MenuEditor
{
 public:
  EnumSettingEditor()
  {
    Application::get().getSettings()->getSetting<tSetting>()->onChange(
        mem_fun(this, &EnumSettingEditor<tSetting>::onSettingChanged));
  }

  void incSetting(int inc) override
  {
    Application::get().getSettings()->getSetting<tSetting>()->incDec(inc, false);
  }

  const std::vector<ustring>& getDisplayStrings() const override
  {
    return Application::get().getSettings()->getSetting<tSetting>()->enumToDisplayString();
  }

  int getSelectedIndex() const override
  {
    auto enabled = Application::get().getSettings()->getSetting<tSetting>()->get();
    return enabled ? 0 : 1;
  }
};
