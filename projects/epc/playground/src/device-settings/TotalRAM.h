#pragma once
#include "Setting.h"

class TotalRAM : public Setting
{
 public:
  explicit TotalRAM(Settings& s);
  bool persistent() const override;
  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;

 private:
  void loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val) override;

  Glib::ustring m_display;
};
