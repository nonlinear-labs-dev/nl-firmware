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
                              OptRefSettings settings);
  Glib::ustring getLongName() const override;
  Glib::ustring getShortName() const override;
  Glib::ustring getInfoText() const override;

  [[nodiscard]] bool isEnabled() const;
  [[nodiscard]] ReturnMode getReturnMode() const;

 private:
  OptRefSettings m_settings;
  void onLocalChanged(const Setting* setting);
  void onRoutingsChanged(const Setting* setting);

 public:
  Layout* createLayout(FocusAndMode focusAndMode) const override;

 private:
  bool m_localIsEnabled = false;
  bool m_routingIsEnabled = false;
  static RoutingSettings::tRoutingIndex getIndex(const ParameterId& id);
  void sendParameterMessage() const override;

 protected:
  nlohmann::json serialize() const override;
  void writeDocProperties(Writer& writer, tUpdateID knownRevision) const override;

  const PhysicalControlParameter* m_sibling;
};