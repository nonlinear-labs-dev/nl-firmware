#include "BaseUnitUIDetail.h"
#include <device-settings/Settings.h>

BaseUnitUIDetail::BaseUnitUIDetail(Settings& settings)
: super(settings, BaseUnitUIDetails::RibbonSelect)
{
}

const std::vector<Glib::ustring>& BaseUnitUIDetail::enumToString() const
{
  static std::vector<Glib::ustring> s_modeNames = {"RibbonSelect", "RibbonMode"};
  return s_modeNames;
}

const std::vector<Glib::ustring>& BaseUnitUIDetail::enumToDisplayString() const
{
  return enumToString();
}
