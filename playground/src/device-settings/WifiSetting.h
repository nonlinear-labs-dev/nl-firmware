#pragma once
#include "BooleanSetting.h"

#include <thread>
#include <condition_variable>

class WifiSetting : public BooleanSetting
{
 public:
  typedef BooleanSetting super;
  explicit WifiSetting(Settings& settings);

  ~WifiSetting() override;

  bool set(tEnum m) override;
  static bool pollAccessPointRunning();

 private:
  void doBackgroundWork();

  std::thread m_bgThread;
  bool m_stopThread = false;
  std::mutex m_mutex;
  std::condition_variable m_condition;
};
