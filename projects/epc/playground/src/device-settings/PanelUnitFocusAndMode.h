#pragma once
#include <proxies/hwui/HWUIEnums.h>
#include "Setting.h"

class PanelUnitFocusAndMode : public Setting
{
 public:
  using Setting::Setting;
  void set(FocusAndMode fam);
  [[nodiscard]] FocusAndMode getState() const;

  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;
  bool persistent() const override;

 private:
  FocusAndMode m_focusAndMode;
};
