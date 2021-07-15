#pragma once
#include "MenuEditor.h"

class HardwareEnableSettingsEditor : public MenuEditor
{
 protected:
  void incSetting(int inc) override;
  const std::vector<Glib::ustring>& getDisplayStrings() const override;
  int getSelectedIndex() const override;
};