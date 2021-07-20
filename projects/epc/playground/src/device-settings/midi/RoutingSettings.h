#pragma once
#include <device-settings/Setting.h>
#include <device-settings/Settings.h>
#include <nltools/messaging/Message.h>

class RoutingSettings : public Setting
{
 public:
  typedef nltools::msg::Setting::MidiSettingsMessage::tRoutingMappings tData;
  typedef nltools::msg::Setting::MidiSettingsMessage::RoutingIndex tRoutingIndex;
  typedef nltools::msg::Setting::MidiSettingsMessage::RoutingAspect tAspectIndex;
  explicit RoutingSettings(Settings& s);
  bool getState(tRoutingIndex hwIdx, tAspectIndex settingIdx) const;
  void setState(tRoutingIndex hwIdx, tAspectIndex settingIdx, bool state);
  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;
  const tData& getRaw() const;
 private:
  tData m_data;
};
