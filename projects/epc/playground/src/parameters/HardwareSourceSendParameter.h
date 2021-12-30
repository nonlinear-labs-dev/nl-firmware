#pragma once
#include <parameters/Parameter.h>
#include <groups/HardwareSourcesGroup.h>
#include <device-settings/midi/RoutingSettings.h>

class PhysicalControlParameter;

class HardwareSourceSendParameter : public Parameter
{
 public:
  HardwareSourceSendParameter(HardwareSourcesGroup* pGroup, PhysicalControlParameter& sibling, const ParameterId& id,
                              const ScaleConverter* converter, double def, int coarseDenominator, int fineDenominator,
                              Settings* settings);
  Layout* createLayout(FocusAndMode focusAndMode) const override;

  [[nodiscard]] bool isLocalEnabled() const;
  [[nodiscard]] ReturnMode getReturnMode() const;
  [[nodiscard]] PhysicalControlParameter* getSiblingParameter() const;
  bool lockingEnabled() const override;
  void onUnselected() override;
  void setCPFromHwui(UNDO::Transaction* transaction, const tControlPositionValue& cpValue) override;
  void setCPFromWebUI(UNDO::Transaction* transaction, const tControlPositionValue& cpValue) override;
  size_t getHash() const override;

 protected:
  nlohmann::json serialize() const override;
  void writeDocProperties(Writer& writer, tUpdateID knownRevision) const override;
  bool shouldWriteDocProperties(tUpdateID knownRevision) const override;


 private:
  void sendParameterMessage() const override;

  void onLocalChanged(const Setting* setting);
  void onRoutingsChanged(const Setting* setting);
  void onSiblingChanged(const Parameter* sibling);
  static RoutingSettings::tRoutingIndex getIndex(const ParameterId& id);
  void updateIsEnabledAndSelectSiblingParameterIfApplicable();

  PhysicalControlParameter& m_sibling;
  Settings* m_settings = nullptr;

  ReturnMode m_returnMode;
  bool m_localIsEnabled = false;
  bool m_routingIsEnabled = false;
  bool m_lastChangedFromHWUI = false;
  bool m_isEnabled = false;
};