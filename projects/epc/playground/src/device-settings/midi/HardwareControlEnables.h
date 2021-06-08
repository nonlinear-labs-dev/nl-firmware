#pragma once
#include <device-settings/Setting.h>
#include <device-settings/Settings.h>
#include <nltools/messaging/Message.h>

class HardwareControlEnables : public Setting
{
 public:
  typedef nltools::msg::Setting::MidiSettingsMessage::HW_INDEX tHWIdx;
  typedef nltools::msg::Setting::MidiSettingsMessage::MAPPING_INDEX tSettingIdx;
  explicit HardwareControlEnables(Settings& s);
  bool getState(tHWIdx hwIdx, tSettingIdx settingIdx) const;
  void setState(tHWIdx hwIdx, tSettingIdx settingIdx, bool state);
  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;

 private:
  std::array<std::array<bool, static_cast<size_t>(tSettingIdx::LENGTH)>, static_cast<size_t>(tHWIdx::LENGTH)> m_data;
};
