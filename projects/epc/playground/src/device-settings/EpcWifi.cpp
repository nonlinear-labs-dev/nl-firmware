#include <playground-helpers.h>
#include <nltools/system/SpawnAsyncCommandLine.h>
#include <nltools/logging/Log.h>
#include <nltools/Types.h>
#include <glibmm.h>
#include "EpcWifi.h"
#include "Application.h"
#include "SSID.h"
#include "Passphrase.h"
#include "WifiSetting.h"

EpcWifi::EpcWifi(const HardwareFeatures& hw, SSID& ssid, Passphrase& password, WifiSetting& wifiEnable)
    : m_currentEpcWifiState(std::nullopt)
    , m_newEpcWifiState(false)
    , m_busy(false)
{
  if(hw.hasEPCWiFi())
  {
    Application::get().getMainContext()->signal_timeout().connect_seconds(
        sigc::mem_fun(this, &EpcWifi::syncCredentials), 2);

    ssid.onChange(sigc::mem_fun(this, &EpcWifi::onSSIDChanged));
    password.onChange(sigc::mem_fun(this, &EpcWifi::onPassphraseChanged));
    wifiEnable.onChange(sigc::mem_fun(this, &EpcWifi::onWifiEnabledChanged));
  }
}

EpcWifi::~EpcWifi() = default;

void EpcWifi::setNewSSID(const Glib::ustring& _newSSID)
{
  m_newSSID = _newSSID;
}

void EpcWifi::setNewPassphrase(const Glib::ustring& _newPassphrase)
{
  m_newPassphrase = _newPassphrase;
}

void EpcWifi::setNewWifiState(bool _state)
{
  m_newEpcWifiState = _state;
}

bool EpcWifi::syncCredentials()
{
  if(m_busy)
    return true;

  updateCredentials(false);
  return true;
}

void EpcWifi::updateCredentials(bool _reload)
{
  m_busy = true;
  if(m_currentPassphrase != m_newPassphrase)
  {
    updatePassphrase();
  }
  else if(m_currentSSID != m_newSSID)
  {
    updateSSID();
  }
  else if(!m_currentEpcWifiState.has_value() || m_currentEpcWifiState != m_newEpcWifiState)
  {
    updateWifiSwitch();
  }
  else if(_reload)
  {
    reloadConnection();
  }
  else
    m_busy = false;
}

void EpcWifi::updateWifiSwitch()
{
  if(m_newEpcWifiState)
    enableConnection();
  else
    disableConnection();
}

void EpcWifi::spawn(const std::vector<std::string>& command, std::function<void(const std::string&)> onSuccess)
{
  if constexpr(!isDevelopmentPC)
  {
    SpawnAsyncCommandLine::spawn(Application::get().getMainContext(), command, onSuccess,
                                 [this](const std::string& e)
                                 {
                                   nltools::Log::error(__FILE__, __LINE__, __PRETTY_FUNCTION__, e);
                                   m_busy = false;
                                 });
  }
}

void EpcWifi::updateSSID()
{
  spawn({ "nmcli", "con", "modify", "C15", "wifi.ssid", m_newSSID },
        [this, p = m_newSSID](auto)
        {
          m_currentSSID = p;
          updateCredentials(true);
        });
}

void EpcWifi::updatePassphrase()
{
  spawn({ "nmcli", "con", "modify", "C15", "802-11-wireless-security.psk", m_newPassphrase },
        [this, p = m_newPassphrase](auto)
        {
          m_currentPassphrase = p;
          updateCredentials(true);
        });
}

void EpcWifi::reloadConnection()
{
  spawn({ "nmcli", "connection", "down", "C15" }, [this](auto) { enableConnection(); });
}

void EpcWifi::enableConnection()
{
  spawn({ "nmcli", "con", "up", "C15" },
        [this](auto)
        {
          m_currentEpcWifiState = true;
          m_busy = false;
        });
}

void EpcWifi::disableConnection()
{
  spawn({ "nmcli", "con", "down", "C15" },
        [this](auto)
        {
          m_currentEpcWifiState = false;
          m_busy = false;
        });
}

void EpcWifi::onPassphraseChanged(const Setting* s)
{
  if(auto passphrase = dynamic_cast<const Passphrase*>(s))
  {
    setNewPassphrase(passphrase->getDisplayString());
  }
}

void EpcWifi::onSSIDChanged(const DeviceInformationItem* s)
{
  if(auto ssid = dynamic_cast<const SSID*>(s))
  {
    setNewSSID(ssid->getDisplayString());
  }
}

void EpcWifi::onWifiEnabledChanged(const Setting* s)
{
  if(auto enable = dynamic_cast<const WifiSetting*>(s))
  {
    setNewWifiState(enable->get() == WifiSettings::Enabled);
  }
}