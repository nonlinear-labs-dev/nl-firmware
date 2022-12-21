#include "SelectedRibbonsSetting.h"
#include <tools/StringTools.h>
#include <presets/EditBuffer.h>

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

void SelectedRibbonsSetting::connectToEditBuffer(EditBuffer* eb)
{
  eb->onPresetLoaded(sigc::mem_fun(this, &SelectedRibbonsSetting::onPresetLoaded));
}

void SelectedRibbonsSetting::onPresetLoaded()
{
  set(SelectedRibbons::Ribbon1_2);
}
