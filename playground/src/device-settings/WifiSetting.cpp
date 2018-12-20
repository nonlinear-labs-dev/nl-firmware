#include <tools/SpawnCommandLine.h>
#include "WifiSetting.h"
#include "DebugLevel.h"

WifiSetting::WifiSetting(Settings& settings)
    : BooleanSetting(settings, WifiSetting::pollAccessPointRunning())
{
}


auto printRet = [](SpawnCommandLine & cmd)
{
  DebugLevel::warning("COut:", cmd.getStdOutput(), "CError:", cmd.getStdError(), "Exit Status:", cmd.getExitStatus());
};

WifiSetting::~WifiSetting() = default;

bool WifiSetting::set(tEnum m)
{
  auto ret = super::set(m);

  if(get()) {
    SpawnCommandLine cmd("su - root -c \"systemctl enable accesspoint && systemctl start accesspoint\"");
    printRet(cmd);
  } else {
    SpawnCommandLine cmd("su - root -c \"systemctl disable accesspoint && systemctl stop accesspoint\"");
    printRet(cmd);
  }
  return ret;
}

bool WifiSetting::pollAccessPointRunning()
{
  SpawnCommandLine cmd("systemctl is-active accesspoint");
  printRet(cmd);
#ifdef DEV_PC
  return true;
#else
  return cmd.getExitStatus() == 0;
#endif
}