#include "WifiSetting.h"
#include <nltools/system/SpawnCommandLine.h>
#include <nltools/Types.h>
#include <nltools/logging/Log.h>
#include <glibmm.h>
#include <arpa/inet.h>

WifiSetting::WifiSetting(UpdateDocumentContributor& settings)
    : super(settings, WifiSettings::Querying)
{
  Glib::MainContext::get_default()->signal_timeout().connect_seconds(
      sigc::mem_fun(this, &WifiSetting::pollAccessPointRunning), 10);
}

bool WifiSetting::set(tEnum m)
{
  auto ret = super::set(m);

  if(!isDevelopmentPC)
  {
    if(get() == WifiSettings::Enabled)
    {
      nltools::Log::warning("async enabling accesspoint");
      Glib::spawn_command_line_async(
          "ssh -o \"StrictHostKeyChecking=no\" root@192.168.10.11 "
          "\"systemctl unmask accesspoint; systemctl enable accesspoint; systemctl start accesspoint\"");
    }
    else if(get() == WifiSettings::Disabled)
    {
      nltools::Log::warning("async disabling accesspoint");
      Glib::spawn_command_line_async(
          "ssh -o \"StrictHostKeyChecking=no\" root@192.168.10.11 "
          "\"systemctl stop accesspoint; systemctl disable accesspoint; systemctl mask accesspoint\"");
    }
  }

  return ret;
}

bool WifiSetting::persistent() const
{
  return false;
}

bool WifiSetting::pollAccessPointRunning()
{
  if(!isDevelopmentPC)
  {
    std::vector<std::string> args{ "/usr/bin/ssh", "-o",        "StrictHostKeyChecking=no", "root@192.168.10.11",
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

  return true;
}

void WifiSetting::onPollReturned(GPid pid, int exitStatus)
{
  if(exitStatus == 0)
  {
    super::set(WifiSettings::Enabled);
  }
  else if(exitStatus == (3 << 8))
  {
    super::set(WifiSettings::Disabled);
  }
}
