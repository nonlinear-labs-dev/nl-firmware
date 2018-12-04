#include <tools/SpawnCommandLine.h>
#include "WifiSetting.h"

WifiSetting::WifiSetting(Settings &settings)
    : BooleanSetting(settings, WifiSetting::pollAccessPointRunning())
{
}

WifiSetting::~WifiSetting() = default;

bool WifiSetting::set(Glib::ustring value)
{
  auto val = value.find("on") != Glib::ustring::npos;
  
  if(val)
    SpawnCommandLine cmd("systemctl enable accesspoint && systemctl start accesspoint");
  else
    SpawnCommandLine cmd("systemctl disable accesspoint && systemctl stop accesspoint");

  return BooleanSetting::set(val ? BooleanSetting::tEnum::BOOLEAN_SETTING_TRUE
                                 : BooleanSetting::tEnum::BOOLEAN_SETTING_FALSE);
}

bool WifiSetting::pollAccessPointRunning()
{
#ifdef DEV_PC
  return true;
#endif
  SpawnCommandLine cmd("systemctl is-active accesspoint ");
  return cmd.getStdOutput().find("active") != Glib::ustring::npos;
}