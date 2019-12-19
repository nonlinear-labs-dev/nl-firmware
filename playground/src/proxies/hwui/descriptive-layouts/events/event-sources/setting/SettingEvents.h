#pragma once
#include "SettingEvent.h"
#include "device-settings/LoadModeSetting.h"

namespace DescriptiveLayouts
{
  using DirectLoadStatus = SettingEvent<bool, LoadModeSetting>;
}