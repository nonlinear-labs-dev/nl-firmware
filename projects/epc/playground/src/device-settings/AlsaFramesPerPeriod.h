#pragma once

#include "Setting.h"

class AlsaFramesPerPeriod : public Setting
{
 private:
  typedef Setting super;

 public:
  explicit AlsaFramesPerPeriod(Settings& parent);
  ~AlsaFramesPerPeriod() override;

  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;

  Glib::ustring getDisplayString() const override;
  void syncExternals(SendReason reason) const override;

  void set(int fpp);

 private:
  void loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val) override;

  constexpr static int c_defaultFramesPerPeriod = 0;
  int m_framesPerPeriod = c_defaultFramesPerPeriod;
};
