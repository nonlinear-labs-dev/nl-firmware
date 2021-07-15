#include "HardwareEnableSettingsEditor.h"

void HardwareEnableSettingsEditor::incSetting(int inc)
{
}

const std::vector<Glib::ustring>& HardwareEnableSettingsEditor::getDisplayStrings() const
{
  static std::vector<Glib::ustring> sTest = {"Foo", "Bar"};
  return sTest;
}

int HardwareEnableSettingsEditor::getSelectedIndex() const
{
  return 0;
}
