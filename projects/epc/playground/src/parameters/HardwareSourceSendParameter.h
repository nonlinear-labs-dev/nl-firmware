#pragma once
#include <parameters/Parameter.h>
#include <groups/HardwareSourcesGroup.h>
#include "ParameterTypes.h"
#include <device-settings/midi/RoutingSettings.h>

class HardwareSourceSendParameter : public Parameter
{
 public:
  HardwareSourceSendParameter(HardwareSourcesGroup* pGroup, const ParameterId& id, const ScaleConverter* converter, double def,
                     int coarseDenominator, int fineDenominator,
                     OptRefSettings settings);

 private:
  OptRefSettings m_settings;
  void onLocalChanged(const Setting* setting);
  void onRoutingsChanged(const Setting* setting);

  bool m_localIsEnabled = false;
  bool m_routingIsEnabled = false;
  RoutingSettings::tRoutingIndex getIndex(const ParameterId& id);
};