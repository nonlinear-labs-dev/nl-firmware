#pragma once

#include <sigc++/sigc++.h>
#include <glibmm/ustring.h>
#include <optional>

class EpcWifi : public sigc::trackable
{
 public:
  EpcWifi();
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

  Glib::ustring m_currentPassphrase, m_newPassphrase;
  Glib::ustring m_currentSSID, m_newSSID;
  std::optional<bool> m_currentEpcWifiState;
  bool m_newEpcWifiState;
  bool m_busy;
};
