#include "WifiSetting.h"
#include <nltools/system/SpawnCommandLine.h>
#include <nltools/Types.h>
#include <nltools/logging/Log.h>
#include <glibmm.h>

WifiSetting::WifiSetting(UpdateDocumentContributor& settings)
    : BooleanSetting(settings, true)
{
  pollAccessPointRunning();
}

bool WifiSetting::set(tEnum m)
{
  auto ret = super::set(m);

  if(!isDevelopmentPC)
  {
    if(get())
    {
      nltools::Log::warning("async enabling accesspoint");
      Glib::spawn_command_line_async(
          "ssh -o \"StrictHostKeyChecking=no\" root@192.168.10.11 "
          "\"systemctl unmask accesspoint; systemctl enable accesspoint; systemctl start accesspoint\"");
    }
    else
    {
      nltools::Log::warning("async disabling accesspoint");
      Glib::spawn_command_line_async(
          "ssh -o \"StrictHostKeyChecking=no\" root@192.168.10.11 "
          "\"systemctl stop accesspoint; systemctl disable accesspoint; systemctl mask accesspoint\"");
    }
  }

  return ret;
}

void WifiSetting::pollAccessPointRunning()
{
  if(!isDevelopmentPC)
  {
    nltools::Log::warning("async polling accesspoint");

    std::vector<std::string> args { "/usr/bin/ssh", "-o",        "StrictHostKeyChecking=no", "root@192.168.10.11",
                                    "systemctl",    "is-active", "accesspoint.service" };

    GPid pid;
    try
    {
      Glib::spawn_async("", args, Glib::SPAWN_DO_NOT_REAP_CHILD, Glib::SlotSpawnChildSetup(), &pid);
      Glib::MainContext::get_default()->signal_child_watch().connect(sigc::mem_fun(this, &WifiSetting::onPollReturned),
                                                                     pid);
    }
    catch(const Glib::SpawnError& e)
    {
      nltools::Log::error(e.what());
    }
  }
}

void WifiSetting::onPollReturned(GPid pid, int exitStatus)
{
  nltools::Log::warning("async polling accesspoint returned with resul  t", exitStatus);
  set(exitStatus == 0 ? BooleanSettings::BOOLEAN_SETTING_TRUE : BooleanSettings::BOOLEAN_SETTING_FALSE);
}
