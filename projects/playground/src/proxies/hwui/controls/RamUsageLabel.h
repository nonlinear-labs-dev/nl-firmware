#pragma once
#include <proxies/hwui/controls/Label.h>
#include <nltools/system/SpawnCommandLine.h>

class RamUsageLabel : public Label
{
 public:
  RamUsageLabel(const Rect &r);

  void refresh();
  StringAndSuffix getText() const override;

 private:
  std::string totalMegsOfRam;
  std::string m_usedRam;
};
