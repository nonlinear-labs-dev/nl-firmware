#pragma once
#include "EnumSetting.h"
#include "nltools/Types.h"

class TestToneTypeSetting : public EnumSetting<TestToneType>
{
 public:
  using EnumSetting<TestToneType>::EnumSetting;
  const std::vector<Glib::ustring> &enumToString() const override;
  const std::vector<Glib::ustring> &enumToDisplayString() const override;
  void loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val) override;
  bool persistent() const override;
  void syncExternals(SendReason reason) const override;
};
