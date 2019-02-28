#include <tools/Throttler.h>

#include <thread>

#pragma once

class AccessPointRestarter
{
 public:
  static AccessPointRestarter &get();
  ~AccessPointRestarter();

  void scheduleRestart();

 private:
  AccessPointRestarter();
  void restartAP();
  void join();

  Throttler m_throttler;
  std::unique_ptr<std::thread> m_thread;
  std::atomic<bool> m_done = false;
};
