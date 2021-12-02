#pragma once
#include <parameters/Parameter.h>
#include <groups/HardwareSourcesGroup.h>
#include "ParameterTypes.h"
#include <device-settings/midi/RoutingSettings.h>

class PhysicalControlParameter;

class HardwareSourceSendParameter : public Parameter
{
 public:
  HardwareSourceSendParameter(HardwareSourcesGroup* pGroup, PhysicalControlParameter* sibling, const ParameterId& id,
                              const ScaleConverter* converter, double def, int coarseDenominator, int fineDenominator,
                              Settings* settings);
  Layout* createLayout(FocusAndMode focusAndMode) const override;

  [[nodiscard]] bool isLocalEnabled() const;
  [[nodiscard]] ReturnMode getReturnMode() const;
  [[nodiscard]] PhysicalControlParameter* getSiblingParameter() const;

 protected:
  nlohmann::json serialize() const override;
  void writeDocProperties(Writer& writer, tUpdateID knownRevision) const override;

  PhysicalControlParameter* m_sibling;

 private:
  void sendParameterMessage() const override;

  void onLocalChanged(const Setting* setting);
  void onRoutingsChanged(const Setting* setting);
  static RoutingSettings::tRoutingIndex getIndex(const ParameterId& id);

  Settings* m_settings;
  bool m_localIsEnabled = false;
  bool m_routingIsEnabled = false;
};