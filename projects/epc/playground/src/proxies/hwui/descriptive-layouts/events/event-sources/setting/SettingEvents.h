#pragma once
#include <device-settings/SplitPointSyncParameters.h>
#include "SettingEvent.h"
#include "device-settings/DirectLoadSetting.h"

namespace DescriptiveLayouts
{
  using DirectLoadStatus = SettingEvent<bool, DirectLoadSetting>;
  using SplitSyncEnabled = SettingEvent<bool, SplitPointSyncParameters>;
  using SplitSyncDisabled = ReverseSettingEvent<bool, SplitPointSyncParameters>;
}