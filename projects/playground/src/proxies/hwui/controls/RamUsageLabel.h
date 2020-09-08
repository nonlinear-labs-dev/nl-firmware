#pragma once
#include <proxies/hwui/controls/Label.h>
#include <nltools/system/SpawnCommandLine.h>

class RamUsageLabel : public Label
{
 public:
  explicit RamUsageLabel(const Rect &r);
  StringAndSuffix getText() const override;
};
