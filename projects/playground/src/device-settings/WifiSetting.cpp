#include <tools/SpawnCommandLine.h>
#include "WifiSetting.h"
#include "DebugLevel.h"

WifiSetting::WifiSetting(UpdateDocumentContributor& settings)
    : BooleanSetting(settings, WifiSetting::pollAccessPointRunning())
    , m_bgThread([&]() {
#ifdef _DEVELOPMENT_PC
      DebugLevel::warning("WiFiSetting not functional, disable DEV_PC to enable WiFi");
#else
      this->doBackgroundWork();
#endif
    })
{
}

auto printRet = [](SpawnCommandLine& cmd) {
  DebugLevel::warning("COut:", cmd.getStdOutput(), "CError:", cmd.getStdError(), "Exit Status:", cmd.getExitStatus());
};

WifiSetting::~WifiSetting()
{
  m_stopThread = true;
  m_condition.notify_all();

  if(m_bgThread.joinable())
    m_bgThread.join();
}

bool WifiSetting::set(tEnum m)
{
  auto ret = super::set(m);
  m_condition.notify_all();
  return ret;
}

void WifiSetting::doBackgroundWork()
{
  std::unique_lock<std::mutex> lock(m_mutex);

  while(!m_stopThread)
  {
    m_condition.wait(lock);

    if(m_stopThread)
      break;

    if(get())
    {
      SpawnCommandLine cmd("su - root -c \"systemctl enable accesspoint && systemctl start accesspoint\"");
      printRet(cmd);
    }
    else
    {
      SpawnCommandLine cmd("su - root -c \"systemctl disable accesspoint && systemctl stop accesspoint\"");
      printRet(cmd);
    }
  }
}

bool WifiSetting::pollAccessPointRunning()
{

#ifdef _DEVELOPMENT_PC
  return true;
#else
  SpawnCommandLine cmd("systemctl is-active accesspoint");
  printRet(cmd);
  return cmd.getExitStatus() == 0;
#endif
}
