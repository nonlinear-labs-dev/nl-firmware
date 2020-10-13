#pragma once
#include <device-settings/SplitPointSyncParameters.h>
#include "SettingEvent.h"
#include "device-settings/DirectLoadSetting.h"

namespace DescriptiveLayouts
{
  using DirectLoadStatus = SettingEvent<bool, DirectLoadSetting>;
  using SplitPointSyncStatus = SettingEvent<bool, SplitPointSyncParameters>;
}