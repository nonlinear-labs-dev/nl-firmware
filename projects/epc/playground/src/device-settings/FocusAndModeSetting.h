#pragma once
#include <proxies/hwui/HWUIEnums.h>
#include "Setting.h"
#include "libundo/undo/Transaction.h"
#include <functional>

class FocusAndModeSetting : public Setting
{
 public:
  using Setting::Setting;

  void set(FocusAndMode fam);
  [[nodiscard]] FocusAndMode getState() const;
  [[nodiscard]] FocusAndMode getOldState() const;

  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;
  bool persistent() const override;

  bool isFocusAndModeFrozen() const;
  void setFocusAndModeFreeze(bool state);

 private:
  void loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val) override;

  static FocusAndMode fixFocusAndModeWithAnys(FocusAndMode in);
  static FocusAndMode removeInvalidsFromSound(FocusAndMode in);

  FocusAndMode removeInvalidStatesFromOldMode(FocusAndMode in) const;
  FocusAndMode restrictFocusAndMode(FocusAndMode in) const;
  FocusAndMode removeEditOnFocusChange(FocusAndMode in) const;

  bool m_focusAndModeFrozen = false;
  FocusAndMode m_focusAndMode;
  FocusAndMode m_oldFocusAndMode;
};
