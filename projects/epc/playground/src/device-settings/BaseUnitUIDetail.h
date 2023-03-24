#pragma once
#include <device-settings/EnumSetting.h>
#include <nltools/Types.h>

class BaseUnitUIDetail : public EnumSetting<BaseUnitUIDetails>
{
 private:
  typedef EnumSetting<BaseUnitUIDetails> super;

 public:
  explicit BaseUnitUIDetail(Settings &settings);
  BaseUnitUIDetail(const BaseUnitUIDetail &other) = delete;
  BaseUnitUIDetail &operator=(const BaseUnitUIDetail &) = delete;
  ~BaseUnitUIDetail() override = default;

 private:
  const std::vector<Glib::ustring> &enumToString() const override;
  const std::vector<Glib::ustring> &enumToDisplayString() const override;
};