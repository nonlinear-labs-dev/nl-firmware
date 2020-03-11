#pragma once
#include <thread>
#include <atomic>

class USBManager
{
 public:
  USBManager();
  ~USBManager();

 private:
  std::atomic_bool m_running { true };
  std::thread m_backgroundThread;

  static bool usbAvailable();
  static bool updateAvailable();
};
