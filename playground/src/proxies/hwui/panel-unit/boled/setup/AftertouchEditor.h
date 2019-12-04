#pragma once

#include "MenuEditor.h"

class Setting;

class AftertouchEditor : public MenuEditor
{
 private:
  typedef MenuEditor super;

 public:
  AftertouchEditor();
  virtual ~AftertouchEditor();

  void incSetting(int inc) override;
  const std::vector<Glib::ustring> &getDisplayStrings() const override;
  int getSelectedIndex() const override;
};
