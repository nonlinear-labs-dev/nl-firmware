#include "USBManager.h"
#include <nltools/messaging/Message.h>
#include <nltools/system/SpawnCommandLine.h>

USBManager::USBManager()
    : m_backgroundThread { [this] {
      while(m_running)
      {
        nltools::msg::USB::USBStatusMessage msg {};
        msg.m_usbAvailable = usbAvailable();
        msg.m_updateAvailable = updateAvailable();
        nltools::msg::send(nltools::msg::EndPoint::Playground, msg);

        std::this_thread::sleep_for(std::chrono::seconds(5));
      }
    } }
{
}

USBManager::~USBManager()
{
  m_running = false;

  if(m_backgroundThread.joinable())
    m_backgroundThread.join();
}

bool USBManager::usbAvailable()
{
  SpawnCommandLine cmd("lsblk");
  return cmd.getStdOutput().find("/mnt/usb-stick") != std::string::npos;
}

bool USBManager::updateAvailable()
{
  SpawnCommandLine cmd("ls /mnt/usb-stick/nonlinear-c15-update.tar");
  return cmd.getExitStatus() == 0;
}
