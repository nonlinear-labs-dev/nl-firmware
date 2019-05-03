#pragma once

#include "SetupEditor.h"
#include "MenuEditor.h"

class WiFiSettingEditor : public MenuEditor
{
 private:
  typedef MenuEditor super;

 public:
  WiFiSettingEditor();
  ~WiFiSettingEditor() override;

  void incSetting(int inc) override;
  const std::vector<ustring> &getDisplayStrings() const override;
  int getSelectedIndex() const override;
};
