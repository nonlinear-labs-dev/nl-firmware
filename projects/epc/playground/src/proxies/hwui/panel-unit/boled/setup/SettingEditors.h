#pragma once

#include "MenuEditor.h"
#include "device-settings/Settings.h"
#include "device-settings/Setting.h"
#include <Application.h>
#include <device-settings/BooleanSetting.h>

//fix for boolean setting
template <typename tSetting> class EnumSettingEditor : public MenuEditor
{
 public:
  EnumSettingEditor()
  {
    getSetting()->onChange(mem_fun(this, &EnumSettingEditor<tSetting>::onSettingChanged));
  }

  void incSetting(int inc) override
  {
    getSetting()->incDec(inc, false);
  }

  const std::vector<Glib::ustring>& getDisplayStrings() const override
  {
    return getSetting()->getDisplayStrings();
  }

  int getSelectedIndex() const override
  {
    return static_cast<size_t>(getSetting()->getEnumIndex());
  }

  tSetting* getSetting() const
  {
    if(Application::exists())
      return Application::get().getSettings()->getSetting<tSetting>();

    return nullptr;
  }
};

template <typename tSetting> class OnOffOrderChangedEnumSettingEditor : public EnumSettingEditor<tSetting>
{
  typedef EnumSettingEditor<tSetting> super;

  [[nodiscard]] const std::vector<Glib::ustring>& getDisplayStrings() const override
  {
    static auto displays = [this] {
      auto raw = super::getSetting()->getDisplayStrings();
      std::reverse(raw.begin(), raw.end());
      return raw;
    }();

    return displays;
  }

  void incSetting(int inc) override
  {
    auto reversedInc = inc * -1;
    super::getSetting()->incDec(reversedInc, false);
  }

  [[nodiscard]] int getSelectedIndex() const override
  {
    static auto numItems = getDisplayStrings().size();
    auto rawIndex = static_cast<size_t>(super::getSetting()->getEnumIndex());
    return (numItems - rawIndex) - 1;
  }
};