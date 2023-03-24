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
  const std::vector<Glib::ustring> &getDisplayStrings() const override;
  int getSelectedIndex() const override;

  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;
};
