#include "DirectLoadSetting.h"
#include <device-settings/Settings.h>

DirectLoadSetting::DirectLoadSetting(Settings &settings)
    : BooleanSetting(settings, false)
{
}

void DirectLoadSetting::load(const Glib::ustring &text, Initiator initiator)
{
  BooleanSetting::load(text, initiator);
}

bool DirectLoadSetting::set(BooleanSettings m)
{
  return BooleanSetting::set(m);
}

void DirectLoadSetting::toggle()
{
  BooleanSetting::toggle();
}
