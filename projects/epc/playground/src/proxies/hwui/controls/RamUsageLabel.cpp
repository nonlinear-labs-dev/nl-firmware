#include <Application.h>
#include <device-settings/UsedRAM.h>
#include <sigc++/adaptors/hide.h>
#include <device-settings/TotalRAM.h>
#include "RamUsageLabel.h"
#include "device-settings/Settings.h"

RamUsageLabel::RamUsageLabel(const Rect &r)
    : Label(r)
{
  auto devInfo = Application::get().getDeviceInformation();
  devInfo->getItem<UsedRAM>()->onChange(sigc::hide<0>(sigc::mem_fun(this, &RamUsageLabel::setDirty)));
}

StringAndSuffix RamUsageLabel::getText() const
{
  auto devInfo = Application::get().getDeviceInformation();
  auto used = devInfo->getItem<UsedRAM>();
  auto total = devInfo->getItem<TotalRAM>();
  return StringAndSuffix { used->getDisplayString() + "/" + total->getDisplayString() + "MB", 0 };
}
