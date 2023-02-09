#pragma once

#include <nltools/messaging/Message.h>
#include "Setting.h"
#include "EpcWifi.h"

class Settings;

class SSID : public Setting
{
 public:
  explicit SSID(Settings& parent, const HardwareFeatures& hwFeatures);
  ~SSID() override;

  Glib::ustring getDisplayString() const override;
  bool persistent() const override;
  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;

 private:
  const bool m_hasEPCWifi = false;
  void updateSSID(const Glib::ustring& str);
  Glib::ustring m_ssid;
};
