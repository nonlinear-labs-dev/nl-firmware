#pragma once
#include "SetupLabel.h"
#include <device-settings/midi/RoutingSettings.h>

class HardwareEnableSettingsView : public SetupLabel
{
 public:
  typedef RoutingSettings::tRoutingIndex tID;
  typedef RoutingSettings::tAspectIndex tAspect;
  explicit HardwareEnableSettingsView(tID id);
};