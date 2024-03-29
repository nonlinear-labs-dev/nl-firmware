#pragma once
#include <device-settings/Setting.h>
#include <device-settings/Settings.h>
#include <nltools/messaging/Message.h>
#include <ParameterId.h>

class RoutingSettings : public Setting
{
 public:
  typedef nltools::msg::Setting::MidiSettingsMessage::tRoutingMappings tData;
  typedef nltools::msg::Setting::MidiSettingsMessage::RoutingIndex tRoutingIndex;
  typedef nltools::msg::Setting::MidiSettingsMessage::RoutingAspect tAspectIndex;

  explicit RoutingSettings(Settings& s);

  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring getDisplayString() const override;
  Glib::ustring save() const override;

  bool getState(const ParameterId& id, tAspectIndex aspect) const;
  bool getState(tRoutingIndex hwIdx, tAspectIndex settingIdx) const;
  void setState(tRoutingIndex hwIdx, tAspectIndex settingIdx, bool state);
  void setAllAspectsForIndex(tRoutingIndex hwIdx, bool state);
  const tData& getRaw() const;
  void setAllValues(bool value);

 private:
  void loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val) override;

  tData m_data;
  bool sanitizeReceiveHWSourcesAndPC();
};
