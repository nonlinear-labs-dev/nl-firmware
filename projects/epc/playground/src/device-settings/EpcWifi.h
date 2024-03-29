#pragma once

#include <sigc++/sigc++.h>
#include <glibmm/ustring.h>
#include <optional>
#include "proxies/hwui/HardwareFeatures.h"

class SSID;
class Passphrase;
class WifiSetting;
class Setting;
class DeviceInformationItem;

class EpcWifi : public sigc::trackable
{
 public:
  EpcWifi(const HardwareFeatures& hw, SSID& ssid, Passphrase& password, WifiSetting& wifiEnable);
  ~EpcWifi();

  void setNewPassphrase(const Glib::ustring& _newPassphrase);
  void setNewSSID(const Glib::ustring& _newSSID);
  void setNewWifiState(bool _state);

 private:
  void spawn(const std::vector<std::string>& command, std::function<void(const std::string&)> onSuccess);

  bool syncCredentials();
  void updateSSID();
  void updatePassphrase();
  void reloadConnection();

  void disableConnection();
  void enableConnection();

  void updateWifiSwitch();
  void updateCredentials(bool _reload);

  void onPassphraseChanged(const Setting* s);
  void onSSIDChanged(const DeviceInformationItem* s);
  void onWifiEnabledChanged(const Setting* s);

  Glib::ustring m_currentPassphrase, m_newPassphrase;
  Glib::ustring m_currentSSID, m_newSSID;
  std::optional<bool> m_currentEpcWifiState;
  bool m_newEpcWifiState;
  bool m_busy;
};
