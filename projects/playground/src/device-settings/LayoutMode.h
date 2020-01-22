#pragma once

#include "Settings.h"
#include "EnumSetting.h"
#include <proxies/hwui/HWUIEnums.h>

class LayoutMode : public EnumSetting<LayoutVersionMode>
{
 public:
  explicit LayoutMode(Settings*);
  const std::vector<Glib::ustring>& enumToString() const override;
  const std::vector<Glib::ustring>& enumToDisplayString() const override;

  void load(const Glib::ustring& text) override;
};
