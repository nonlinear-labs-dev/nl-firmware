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

  void setUndoable(UNDO::Transaction* transaction, FocusAndMode newFam);
  bool isFocusAndModeFrozen() const;
  void setFocusAndModeFreeze(bool state);

 private:
  static FocusAndMode fixFocusAndModeWithAnys(FocusAndMode in);
  static FocusAndMode removeInvalidsFromSound(FocusAndMode in);

  FocusAndMode restrictFocusAndMode(FocusAndMode in) const;
  FocusAndMode removeEditOnFocusChange(FocusAndMode in) const;

  bool m_focusAndModeFrozen = false;
  FocusAndMode m_focusAndMode;
  FocusAndMode m_oldFocusAndMode;
};
