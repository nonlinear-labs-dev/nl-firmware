#pragma once
#include <device-settings/EnumSetting.h>
#include <nltools/enums/EnumTools.h>

ENUM(SelectedRibbons, int, Ribbon1_2, Ribbon3_4);

class SelectedRibbonsSetting : public EnumSetting<SelectedRibbons>
{
 public:
  explicit SelectedRibbonsSetting(UpdateDocumentContributor& parent);
  const std::vector<Glib::ustring>& enumToString() const override;
  const std::vector<Glib::ustring>& enumToDisplayString() const override;
};
