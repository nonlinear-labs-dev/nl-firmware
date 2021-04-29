#pragma once

#include <nltools/messaging/Message.h>
#include "Setting.h"
#include "Wifi.h"

class Settings;

class SSID : public Setting
{
 public:
  Glib::ustring getDisplayString() const override;

  explicit SSID(Settings& parent, const std::shared_ptr<Wifi>& shrd_ptr);
  ~SSID() override;

  bool persistent() const override;
  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;

 private:
  void updateSSID(const Glib::ustring& str);
  Glib::ustring m_ssid;
  const std::shared_ptr<Wifi> m_wifi
};
