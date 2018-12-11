#include <tools/SpawnCommandLine.h>
#include "WifiSetting.h"
#include "DebugLevel.h"

WifiSetting::WifiSetting(Settings &settings)
    : BooleanSetting(settings, WifiSetting::pollAccessPointRunning())
{
  auto bState = get();
  auto wifiState = WifiSetting::pollAccessPointRunning();
  if(bState && !wifiState)
  {
    set("on");
  }
  else if(!bState && wifiState)
  {
    set("off");
  }
}

WifiSetting::~WifiSetting() = default;

bool WifiSetting::set(Glib::ustring value)
{
  auto printRet = [](SpawnCommandLine& cmd) {
      DebugLevel::warning("COut:", cmd.getStdOutput(), "CError:", cmd.getStdError());
  };

  bool on = value.find("on") != Glib::ustring::npos;
  bool off = value.find("off") != Glib::ustring::npos;

  if(on) {
    SpawnCommandLine cmd("systemctl enable accesspoint && systemctl start accesspoint");
    printRet(cmd);
  }
  else if(off) {
    SpawnCommandLine cmd("systemctl disable accesspoint && systemctl stop accesspoint");
    printRet(cmd);
  }

  return BooleanSetting::set(on ? BooleanSetting::tEnum::BOOLEAN_SETTING_TRUE
                                : BooleanSetting::tEnum::BOOLEAN_SETTING_FALSE);
}

bool WifiSetting::pollAccessPointRunning()
{
#ifdef DEV_PC
  return true;
#endif
  SpawnCommandLine cmd("systemctl is-active accesspoint");
  return cmd.getStdOutput().find("active") != Glib::ustring::npos;
}