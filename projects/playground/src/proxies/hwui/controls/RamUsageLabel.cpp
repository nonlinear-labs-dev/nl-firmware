#include "RamUsageLabel.h"

RamUsageLabel::RamUsageLabel(const Rect &r)
    : Label(r)
{
  static SpawnCommandLine cmd("free --mega | awk 'FNR == 2 {print $2}'");
  totalMegsOfRam = cmd.getStdOutput();
}

void RamUsageLabel::refresh()
{
  SpawnCommandLine cmd("free --mega | awk 'FNR == 2 {print $3}'");
  m_usedRam = cmd.getStdOutput();
}

Label::StringAndSuffix RamUsageLabel::getText() const
{
  return { m_usedRam + "/" + totalMegsOfRam + "M", 0 };
}
