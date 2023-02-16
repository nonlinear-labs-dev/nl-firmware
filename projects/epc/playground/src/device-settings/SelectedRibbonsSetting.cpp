#include "SelectedRibbonsSetting.h"
#include <tools/StringTools.h>

SelectedRibbonsSetting::SelectedRibbonsSetting(UpdateDocumentContributor& parent)
    : EnumSetting<SelectedRibbons>(parent, SelectedRibbons::Ribbon1_2)
{
}

const std::vector<Glib::ustring>& SelectedRibbonsSetting::enumToString() const
{
  static auto ret = StringTools::convert(getAllStrings<SelectedRibbons>());
  return ret;
}

const std::vector<Glib::ustring>& SelectedRibbonsSetting::enumToDisplayString() const
{
  static auto ret = StringTools::convert(getAllStrings<SelectedRibbons>());
  return ret;
}
