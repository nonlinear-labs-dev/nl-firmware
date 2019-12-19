#pragma once
#include "SettingEvent.h"
#include "device-settings/LoadPresetSetting.h"

namespace DescriptiveLayouts
{
  using DirectLoadStatus = SettingEvent<bool, LoadPresetSetting>;
}