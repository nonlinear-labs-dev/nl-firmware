#include "LoadToPartSetting.h"
#include <device-settings/Settings.h>

LoadToPartSetting::LoadToPartSetting(Settings& settings)
    : BooleanSetting(settings, false)
{
}
