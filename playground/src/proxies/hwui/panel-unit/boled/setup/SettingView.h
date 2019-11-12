#pragma once

#include "SetupLabel.h"
#include <Application.h>
#include "device-settings/Settings.h"
#include "device-settings/Setting.h"

template <typename tSetting> class SettingView : public SetupLabel
{
 public:
  SettingView()
      : SetupLabel("", { 0, 0, 0, 0 })
  {
    Application::get().getSettings()->getSetting<tSetting>()->onChange(
        mem_fun(this, &SettingView<tSetting>::onSettingChanged));
  }

  void onSettingChanged(const Setting *s)
  {
    auto v = dynamic_cast<const tSetting *>(s);
    setText(v->getDisplayString());
  }
};