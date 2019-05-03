#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include "SetupEditor.h"

class Setting;

class MenuEditor : public ControlWithChildren, public SetupEditor
{
 private:
  typedef ControlWithChildren super;

 public:
  MenuEditor();
  virtual ~MenuEditor();

  void setPosition(const Rect &r) override;
  bool onButton(int i, bool down, ButtonModifiers modifiers) override;
  bool onRotary(int inc, ButtonModifiers modifiers) override;

 protected:
  virtual void incSetting(int inc) = 0;
  virtual const std::vector<ustring> &getDisplayStrings() const = 0;
  virtual int getSelectedIndex() const = 0;
  void onSettingChanged(const Setting *s);
  void updateOnSettingChanged();
};
