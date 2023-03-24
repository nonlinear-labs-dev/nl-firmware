#pragma once

#include <nltools/messaging/Message.h>
#include <nltools/system/SerializedAsyncCommands.h>

class WiFiManager
{
 public:
  WiFiManager();

 private:
  void saveConfig();
  void scheduleRestart();
  void enableAndStartAP();
  void disableAndStopAP();
  void startDEBUGWifi();

  std::string m_lastSeenSSID {};
  std::string m_lastSeenPassword {};

  SerializedAsyncCommands m_asyncCommands;

  constexpr static auto c_ssidPattern = "ssid";
  constexpr static auto c_pwPattern = "wpa_passphrase";
  constexpr static auto c_fileName = "/etc/hostapd.conf";
};
