#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include "MenuEditor.h"

class Setting;
class PedalType;

class PedalEditor : public MenuEditor
{
 private:
  typedef MenuEditor super;

 public:
  explicit PedalEditor(PedalType* m);
  ~PedalEditor() override;
  void setPosition(const Rect& r) override;
  void incSetting(int inc) override;
  const std::vector<Glib::ustring>& getDisplayStrings() const override;
  int getSelectedIndex() const override;

  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;

 private:
  PedalType* m_mode;
};
