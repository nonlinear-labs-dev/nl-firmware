#pragma once
#include <parameters/Parameter.h>
#include <groups/HardwareSourcesGroup.h>
#include <device-settings/midi/RoutingSettings.h>

class PhysicalControlParameter;

class HardwareSourceSendParameter : public Parameter
{
 public:
  HardwareSourceSendParameter(HardwareSourcesGroup* pGroup, PhysicalControlParameter& sibling, const ParameterId& id,
                              Settings* settings);
  void init(Settings* settings);
  Layout* createLayout(FocusAndMode focusAndMode) const override;

  [[nodiscard]] bool isLocalEnabled() const;
  [[nodiscard]] bool isAssigned() const;
  [[nodiscard]] ReturnMode getReturnMode() const;
  [[nodiscard]] PhysicalControlParameter* getSiblingParameter() const;
  bool lockingEnabled() const override;
  void onUnselected() override;
  void setCPFromHwui(UNDO::Transaction* transaction, const tControlPositionValue& cpValue) override;
  void setCPFromWebUI(UNDO::Transaction* transaction, const tControlPositionValue& cpValue) override;
  size_t getHash() const override;
  void loadFromPreset(UNDO::Transaction* transaction, const tControlPositionValue& value) override;
  bool isChangedFromLoaded() const override;
  bool isValueChangedFromLoaded() const override;

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
  void updateAndNotifyLocalEnableStateAndSelectSiblingParameterIfApplicable();

  PhysicalControlParameter& m_sibling;
  Settings* m_settings = nullptr;

  ReturnMode m_returnMode = ReturnMode::None;
  bool m_localIsEnabled;
  bool m_routingIsEnabled;
  bool m_lastChangedFromHWUI = false;
  bool m_isEnabled;
};