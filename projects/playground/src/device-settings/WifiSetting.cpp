#include <nltools/system/SpawnCommandLine.h>
#include <nltools/system/SpawnAsyncCommandLine.h>
#include <Application.h>
#include "WifiSetting.h"
#include "DebugLevel.h"

WifiSetting::WifiSetting(UpdateDocumentContributor& settings)
    : BooleanSetting(settings, WifiSetting::pollAccessPointRunning())
{
}

bool WifiSetting::set(tEnum m)
{
  auto ret = super::set(m);

  Application::get().stopWatchDog();
#ifndef _DEVELOPMENT_PC
  if(get())
  {
    SpawnAsyncCommandLine cmd("ssh -o \"StrictHostKeyChecking=no\" root@192.168.10.11 'systemctl enable accesspoint && "
                              "systemctl start accesspoint'");
  }
  else
  {
    SpawnAsyncCommandLine cmd(
        "ssh -o \"StrictHostKeyChecking=no\" root@192.168.10.11 'systemctl disable accesspoint && "
        "systemctl stop accesspoint'");
  }
#endif

  Application::get().runWatchDog();
  return ret;
}

bool WifiSetting::pollAccessPointRunning()
{

#ifdef _DEVELOPMENT_PC
  return true;
#else
  SpawnCommandLine cmd("ssh -o \"StrictHostKeyChecking=no\" root@192.168.10.11 'systemctl is-active accesspoint'");
  return cmd.getExitStatus() == 0;
#endif
}
